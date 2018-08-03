# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.nanocat.Debug:
PostBuild.nanomsg.Debug: /Users/liwei/github/uv_demo/nanomsg/build/Debug/nanocat
/Users/liwei/github/uv_demo/nanomsg/build/Debug/nanocat:\
	/Users/liwei/github/uv_demo/nanomsg/build/Debug/libnanomsg.5.1.0.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/Debug/nanocat


PostBuild.nanomsg.Debug:
/Users/liwei/github/uv_demo/nanomsg/build/Debug/libnanomsg.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/Debug/libnanomsg.dylib


PostBuild.nanocat.Release:
PostBuild.nanomsg.Release: /Users/liwei/github/uv_demo/nanomsg/build/Release/nanocat
/Users/liwei/github/uv_demo/nanomsg/build/Release/nanocat:\
	/Users/liwei/github/uv_demo/nanomsg/build/Release/libnanomsg.5.1.0.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/Release/nanocat


PostBuild.nanomsg.Release:
/Users/liwei/github/uv_demo/nanomsg/build/Release/libnanomsg.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/Release/libnanomsg.dylib


PostBuild.nanocat.MinSizeRel:
PostBuild.nanomsg.MinSizeRel: /Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/nanocat
/Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/nanocat:\
	/Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/libnanomsg.5.1.0.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/nanocat


PostBuild.nanomsg.MinSizeRel:
/Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/libnanomsg.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/libnanomsg.dylib


PostBuild.nanocat.RelWithDebInfo:
PostBuild.nanomsg.RelWithDebInfo: /Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/nanocat
/Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/nanocat:\
	/Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/libnanomsg.5.1.0.dylib
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/nanocat


PostBuild.nanomsg.RelWithDebInfo:
/Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/libnanomsg.dylib:
	/bin/rm -f /Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/libnanomsg.dylib




# For each target create a dummy ruleso the target does not have to exist
/Users/liwei/github/uv_demo/nanomsg/build/Debug/libnanomsg.5.1.0.dylib:
/Users/liwei/github/uv_demo/nanomsg/build/MinSizeRel/libnanomsg.5.1.0.dylib:
/Users/liwei/github/uv_demo/nanomsg/build/RelWithDebInfo/libnanomsg.5.1.0.dylib:
/Users/liwei/github/uv_demo/nanomsg/build/Release/libnanomsg.5.1.0.dylib:
