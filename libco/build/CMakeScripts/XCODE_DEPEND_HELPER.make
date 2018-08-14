# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.colib_shared.Debug:
/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/libcolib.dylib


PostBuild.colib_static.Debug:
/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a


PostBuild.example_closure.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_closure
/Users/liwei/github/uv_demo/libco/build/Debug/example_closure:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_closure


PostBuild.example_cond.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_cond
/Users/liwei/github/uv_demo/libco/build/Debug/example_cond:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_cond


PostBuild.example_copystack.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_copystack
/Users/liwei/github/uv_demo/libco/build/Debug/example_copystack:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_copystack


PostBuild.example_echocli.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_echocli
/Users/liwei/github/uv_demo/libco/build/Debug/example_echocli:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_echocli


PostBuild.example_echosvr.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_echosvr
/Users/liwei/github/uv_demo/libco/build/Debug/example_echosvr:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_echosvr


PostBuild.example_poll.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_poll
/Users/liwei/github/uv_demo/libco/build/Debug/example_poll:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_poll


PostBuild.example_setenv.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_setenv
/Users/liwei/github/uv_demo/libco/build/Debug/example_setenv:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_setenv


PostBuild.example_specific.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_specific
/Users/liwei/github/uv_demo/libco/build/Debug/example_specific:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_specific


PostBuild.example_thread.Debug:
PostBuild.colib_static.Debug: /Users/liwei/github/uv_demo/libco/build/Debug/example_thread
/Users/liwei/github/uv_demo/libco/build/Debug/example_thread:\
	/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Debug/example_thread


PostBuild.colib_shared.Release:
/Users/liwei/github/uv_demo/libco/build/Release/libcolib.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/libcolib.dylib


PostBuild.colib_static.Release:
/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/libcolib.a


PostBuild.example_closure.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_closure
/Users/liwei/github/uv_demo/libco/build/Release/example_closure:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_closure


PostBuild.example_cond.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_cond
/Users/liwei/github/uv_demo/libco/build/Release/example_cond:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_cond


PostBuild.example_copystack.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_copystack
/Users/liwei/github/uv_demo/libco/build/Release/example_copystack:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_copystack


PostBuild.example_echocli.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_echocli
/Users/liwei/github/uv_demo/libco/build/Release/example_echocli:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_echocli


PostBuild.example_echosvr.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_echosvr
/Users/liwei/github/uv_demo/libco/build/Release/example_echosvr:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_echosvr


PostBuild.example_poll.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_poll
/Users/liwei/github/uv_demo/libco/build/Release/example_poll:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_poll


PostBuild.example_setenv.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_setenv
/Users/liwei/github/uv_demo/libco/build/Release/example_setenv:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_setenv


PostBuild.example_specific.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_specific
/Users/liwei/github/uv_demo/libco/build/Release/example_specific:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_specific


PostBuild.example_thread.Release:
PostBuild.colib_static.Release: /Users/liwei/github/uv_demo/libco/build/Release/example_thread
/Users/liwei/github/uv_demo/libco/build/Release/example_thread:\
	/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/Release/example_thread


PostBuild.colib_shared.MinSizeRel:
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.dylib


PostBuild.colib_static.MinSizeRel:
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a


PostBuild.example_closure.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_closure
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_closure:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_closure


PostBuild.example_cond.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_cond
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_cond:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_cond


PostBuild.example_copystack.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_copystack
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_copystack:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_copystack


PostBuild.example_echocli.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_echocli
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_echocli:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_echocli


PostBuild.example_echosvr.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_echosvr
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_echosvr:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_echosvr


PostBuild.example_poll.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_poll
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_poll:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_poll


PostBuild.example_setenv.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_setenv
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_setenv:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_setenv


PostBuild.example_specific.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_specific
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_specific:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_specific


PostBuild.example_thread.MinSizeRel:
PostBuild.colib_static.MinSizeRel: /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_thread
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_thread:\
	/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/MinSizeRel/example_thread


PostBuild.colib_shared.RelWithDebInfo:
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.dylib


PostBuild.colib_static.RelWithDebInfo:
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a:
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a


PostBuild.example_closure.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_closure
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_closure:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_closure


PostBuild.example_cond.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_cond
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_cond:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_cond


PostBuild.example_copystack.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_copystack
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_copystack:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_copystack


PostBuild.example_echocli.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_echocli
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_echocli:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_echocli


PostBuild.example_echosvr.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_echosvr
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_echosvr:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_echosvr


PostBuild.example_poll.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_poll
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_poll:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_poll


PostBuild.example_setenv.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_setenv
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_setenv:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_setenv


PostBuild.example_specific.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_specific
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_specific:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_specific


PostBuild.example_thread.RelWithDebInfo:
PostBuild.colib_static.RelWithDebInfo: /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_thread
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_thread:\
	/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a
	/bin/rm -f /Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/example_thread




# For each target create a dummy ruleso the target does not have to exist
/Users/liwei/github/uv_demo/libco/build/Debug/libcolib.a:
/Users/liwei/github/uv_demo/libco/build/MinSizeRel/libcolib.a:
/Users/liwei/github/uv_demo/libco/build/RelWithDebInfo/libcolib.a:
/Users/liwei/github/uv_demo/libco/build/Release/libcolib.a:
