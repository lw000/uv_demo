//
//  main.cpp
//  nanomsg-test
//
//  Created by 李伟 on 2018/8/3.
//  Copyright © 2018年 李伟. All rights reserved.
//

#include <reqrep.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>  /* For htonl and ntohl */
#include <unistd.h>

#include <nn.h>
#include <pubsub.h>

/*  The server runs forever. */
int server(const char *url)
{
    int fd;
    
    /*  Create the socket. */
    fd = nn_socket (AF_SP, NN_PUB);
    if (fd < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        return (-1);
    }
    
    /*  Bind to the URL.  This will bind to the address and listen
     synchronously; new clients will be accepted asynchronously
     without further action from the calling program. */
    
    if (nn_bind (fd, url) < 0) {
        fprintf (stderr, "nn_bind: %s\n", nn_strerror (nn_errno ()));
        nn_close (fd);
        return (-1);
    }
    
    /*  Now we can just publish results.  Note that there is no explicit
     accept required.  We just start writing the information. */
    
    for (;;) {
        uint8_t msg[2 * sizeof (uint32_t)];
        uint32_t secs, subs;
        int rc;
        
        secs = (uint32_t) time (NULL);
        subs = (uint32_t) nn_get_statistic (fd, NN_STAT_CURRENT_CONNECTIONS);
        
        secs = htonl (secs);
        subs = htonl (subs);
        
        memcpy (msg, &secs, sizeof (secs));
        memcpy (msg + sizeof (secs), &subs, sizeof (subs));
        
        rc = nn_send (fd, msg, sizeof (msg), 0);
        if (rc < 0) {
            /*  There are several legitimate reasons this can fail.
             We note them for debugging purposes, but then ignore
             otherwise. */
            fprintf (stderr, "nn_send: %s (ignoring)\n",
                     nn_strerror (nn_errno ()));
        }
        sleep(10);
    }
    
    /* NOTREACHED */
    nn_close (fd);
    return (-1);
}

/*  The client runs in a loop, displaying the content. */
int client (const char *url)
{
    int fd;
    int rc;
    
    fd = nn_socket (AF_SP, NN_SUB);
    if (fd < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        return (-1);
    }
    
    if (nn_connect (fd, url) < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        nn_close (fd);
        return (-1);
    }
    
    /*  We want all messages, so just subscribe to the empty value. */
    if (nn_setsockopt (fd, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
        fprintf (stderr, "nn_setsockopt: %s\n", nn_strerror (nn_errno ()));
        nn_close (fd);
        return (-1);
    }
    
    for (;;) {
        uint8_t msg[2 * sizeof (uint32_t)];
        char hhmmss[9];  /* HH:MM:SS\0 */
        uint32_t subs, secs;
        time_t t;
        
        rc = nn_recv (fd, msg, sizeof (msg), 0);
        if (rc < 0) {
            fprintf (stderr, "nn_recv: %s\n", nn_strerror (nn_errno ()));
            break;
        }
        if (rc != sizeof (msg)) {
            fprintf (stderr, "nn_recv: got %d bytes, wanted %d\n",
                     rc, (int)sizeof (msg));
            break;
        }
        memcpy (&secs, msg, sizeof (secs));
        memcpy (&subs, msg + sizeof (secs), sizeof (subs));
        
        t = (time_t) ntohl(secs);
        strftime (hhmmss, sizeof (hhmmss), "%T", localtime (&t));
        
        printf ("%s <pid %u> There are %u clients connected.\n", hhmmss,
                (unsigned) getpid(), (unsigned) ntohl(subs));
    }
    
    nn_close (fd);
    return (-1);
}

int main (int argc, char **argv)
{
    int rc;
    if ((argc == 3) && (strcmp (argv[2], "-s") == 0)) {
        rc = server (argv[1]);
    } else if (argc == 2) {
        rc = client (argv[1]);
    } else {
        fprintf (stderr, "Usage: %s <url> [-s]\n", argv[0]);
        exit (EXIT_FAILURE);
    }
    exit (rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
