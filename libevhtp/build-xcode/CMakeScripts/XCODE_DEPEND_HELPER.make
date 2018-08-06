# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.evhtp.Debug:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a


PostBuild.test.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test


PostBuild.test_basic.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_basic
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_basic:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_basic


PostBuild.test_client.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_client
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_client:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_client


PostBuild.test_perf.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_perf
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_perf:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_perf


PostBuild.test_proxy.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_proxy
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_proxy:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_proxy


PostBuild.test_query.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_query
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_query:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_query


PostBuild.test_vhost.Debug:
PostBuild.evhtp.Debug: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_vhost
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_vhost:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Debug/test_vhost


PostBuild.evhtp.Release:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a


PostBuild.test.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test


PostBuild.test_basic.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_basic
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_basic:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_basic


PostBuild.test_client.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_client
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_client:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_client


PostBuild.test_perf.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_perf
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_perf:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_perf


PostBuild.test_proxy.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_proxy
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_proxy:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_proxy


PostBuild.test_query.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_query
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_query:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_query


PostBuild.test_vhost.Release:
PostBuild.evhtp.Release: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_vhost
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_vhost:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/Release/test_vhost


PostBuild.evhtp.MinSizeRel:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a


PostBuild.test.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test


PostBuild.test_basic.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_basic
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_basic:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_basic


PostBuild.test_client.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_client
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_client:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_client


PostBuild.test_perf.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_perf
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_perf:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_perf


PostBuild.test_proxy.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_proxy
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_proxy:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_proxy


PostBuild.test_query.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_query
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_query:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_query


PostBuild.test_vhost.MinSizeRel:
PostBuild.evhtp.MinSizeRel: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_vhost
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_vhost:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/MinSizeRel/test_vhost


PostBuild.evhtp.RelWithDebInfo:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a


PostBuild.test.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test


PostBuild.test_basic.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_basic
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_basic:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_basic


PostBuild.test_client.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_client
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_client:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_client


PostBuild.test_perf.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_perf
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_perf:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_perf


PostBuild.test_proxy.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_proxy
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_proxy:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_proxy


PostBuild.test_query.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_query
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_query:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_query


PostBuild.test_vhost.RelWithDebInfo:
PostBuild.evhtp.RelWithDebInfo: /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_vhost
/Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_vhost:\
	/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a\
	/usr/local/lib/libevent.dylib\
	/usr/local/lib/libevent_pthreads.dylib\
	/usr/lib/libdl.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/libevhtp/build-xcode/examples/RelWithDebInfo/test_vhost




# For each target create a dummy ruleso the target does not have to exist
/Users/liwei/github/uv_demo/libevhtp/build-xcode/Debug/libevhtp.a:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/MinSizeRel/libevhtp.a:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/RelWithDebInfo/libevhtp.a:
/Users/liwei/github/uv_demo/libevhtp/build-xcode/Release/libevhtp.a:
/usr/lib/libdl.dylib:
/usr/local/lib/libevent.dylib:
/usr/local/lib/libevent_pthreads.dylib:
