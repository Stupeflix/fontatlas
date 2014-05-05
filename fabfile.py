from fabric.api import local, lcd, task
import os
import os.path
import stupeflix.deployment.ios as ios

HERE = os.path.dirname(__file__)
DEVEL = os.path.abspath(os.path.join(HERE, ".."))

FRAMEWORK_PATH=os.path.join(HERE, "frameworks")
FRAMEWORK_NAME="fontatlas"

PROJECT_NAME = "fontatlas"
INCLUDES="include/*"
LIBRARIES = ["fontatlas"]

BUILD_SUFFIX="-lagunas"

FREETYPE_FLAGS="""-D FREETYPE_INCLUDE_DIR_freetype2:PATH=%s/freetype2-ios/include/freetype  \
-D FREETYPE_INCLUDE_DIR_ft2build:PATH=%s/freetype2-ios/include  \
-D FREETYPE_LIBRARY:PATH=%s/ios/apps/dependencies/lib/ios/libfreetype.a \
""" % (DEVEL, DEVEL, DEVEL)

PNG_FLAGS="""-D PNG_PNG_INCLUDE_DIR:PATH=%s/libpng-ios/include/ \
-D PNG_LIBRARIES:PATH=%s/libpng-ios/build/libpng.a \
""" % (DEVEL, DEVEL)

LIB_FLAGS = PNG_FLAGS + FREETYPE_FLAGS

COMMON_FLAGS="""-D CMAKE_C_COMPILER_FORCED:BOOL=ON \
-D CMAKE_CXX_COMPILER_FORCED:BOOL=ON \
"""

CMAKE_CXX_FLAGS=" -stdlib=libstdc++ -std=c++11 -ftree-vectorize -fvisibility-inlines-hidden -pipe -no-cpp-precomp "

def sdkroot_get(type):
    if type == "iPhoneOS":
        PRINT_INDEX=2
    elif type == "iPhoneSimulator":
        PRINT_INDEX=4
    else:
        raise Exception("Unknown type")

    SDKVER = local("""xcodebuild -showsdks | fgrep %s | tail -n 1 | awk '{print $%d}'""" % (type.lower(), PRINT_INDEX), capture = True)
    DEVROOT = local("xcode-select -print-path", capture = True) + "/Platforms/%s.platform/Developer" % type
    return os.path.join(DEVROOT, "SDKs/%s%s.sdk" % (type, SDKVER))

DEVICE_FLAGS="""-D CMAKE_CXX_FLAGS:STRING=" %s -mno-thumb -arch arm64 -arch armv7 -arch armv7s -miphoneos-version-min=7.0 " \
-D CMAKE_OSX_SYSROOT:STRING=%s \
-D CMAKE_XCODE_ATTRIBUTE_CLANG_C_LIBRARY:STRING=libstdc++ \
-D CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY:STRING=libstdc++ \
-D CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD:STRING=c++11 \
-D CMAKE_XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD:STRING=c99 \
-D CMAKE_XCODE_ATTRIBUTE_CLANG_C_LIBRARY:STRING= \
-D CMAKE_DEBUG_POSTFIX:STRING='' \
-D CMAKE_OSX_ARCHITECTURES:STRING="arm64;armv7;armv7s" \
""" % (CMAKE_CXX_FLAGS, sdkroot_get("iPhoneOS"))

SIMULATOR_FLAGS="""-D CMAKE_CXX_FLAGS:STRING=" %s -arch i386 -arch x86_64  -mios-simulator-version-min=7.0" \
-D CMAKE_OSX_SYSROOT:STRING=%s \
-D CMAKE_OSX_ARCHITECTURES:STRING="i386" \
""" % (CMAKE_CXX_FLAGS, sdkroot_get("iPhoneSimulator"))

def run_cmake(BUILD):
    flags = COMMON_FLAGS + LIB_FLAGS
    if BUILD == "device":
        flags += DEVICE_FLAGS
    elif BUILD == "simulator":
        flags += SIMULATOR_FLAGS
    else:
        raise Exception("BUILD %s not supported yet." % BUILD)
    local("cmake -G Xcode %s .." % flags)

PATCH_SRC='"/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk"'
PATCH_DEST='iphoneos'

def replace_in_file(filename, src, dest):
    s = open(filename).read()
    s = s.replace(src, dest)
    f = open(filename, "w")
    f.write(s)

@task
def build(BUILD, release="true", reset="true", target=None):
    if release.lower() == "true":
        CONFIG_TYPE = "Release"
    else:
        CONFIG_TYPE= "Debug"
        
    BUILD_TYPES = ["simulator", "device", "android"]
    if BUILD not in BUILD_TYPES:
        raise Exception("Unknown build type %s, should be in [%s]" % (build, ", ".join(BUILD_TYPES)))
    if BUILD != "android":
        BUILD_PREFIX="ios-"
    else:
        BUILD_PREFIX=""

    BUILD_PATH = "build-%s%s%s" % (BUILD_PREFIX, BUILD, BUILD_SUFFIX)
    local("mkdir -p %s" % BUILD_PATH)

    if target == None:
        TARGETS = LIBRARIES
    else:
        TARGETS = [target]

    if reset == "true":
        local("rm -fr %s" % BUILD_PATH)
        local("mkdir -p %s" % BUILD_PATH)

    with lcd(BUILD_PATH):
        if target == None:
            run_cmake(BUILD)
        proj_file = os.path.join(BUILD_PATH,  PROJECT_NAME + ".xcodeproj/project.pbxproj")
        replace_in_file(proj_file, PATCH_SRC, PATCH_DEST)
        replace_in_file(proj_file, "-miphoneos-version-min=3.1", "")
        replace_in_file(proj_file, "-arch armv6", "")

        for TARGET in TARGETS:
            if BUILD == "simulator":
                typeInfo = "-sdk iphonesimulator -arch i386 -arch x86_64"
            elif BUILD == "device":
                typeInfo = "-sdk iphoneos  -arch arm64 -arch armv7 -arch armv7s "
            if BUILD in ["simulator", "device"]:
                local("xcodebuild -IDEBuildOperationMaxNumberOfConcurrentCompileTasks=8 -project " + PROJECT_NAME + ".xcodeproj %s -configuration %s -target %s" % (typeInfo, CONFIG_TYPE, TARGET))
            else:
                local("%s/ndk-build -j 8 NDK_APPLICATION_MK=Application.mk" % NDK)
                
@task
def create_framework(platform="ios"):
    if not platform == "ios":
       raise Exception("Platform should be ios")

    # Create structure
    path = ios.create_framework_structure(FRAMEWORK_PATH, FRAMEWORK_NAME)

    targets = ["device", "simulator"]

    # Build big fatlib
    command = "xcrun libtool -static -o "+os.path.join(path,"Versions/Current/",FRAMEWORK_NAME)
    for name in LIBRARIES:
        for target in targets:
            src_name = "lib%s.a" % name
            command += " " + os.path.join(HERE, "build-ios-%s%s/Release-iphone%s" % (target, BUILD_SUFFIX, "os" if target == "device" else target), src_name)

    local(command)

    # Copy headers
    local("cp -r %s %s" % (os.path.join(HERE, INCLUDES), os.path.join(path, "Headers")))
    ios.finalize_framework_structure(FRAMEWORK_PATH, FRAMEWORK_NAME)

@task
def copy_framework(platform="ios", target = "release"):
    DESTS = ["../iosframeworks/frameworks/%s/" % target]
    for framework in [FRAMEWORK_NAME]:
        frameworkPath = os.path.join(HERE, "frameworks/%s.framework" % framework)
        for d in DESTS:
            local("rm -fr %s " % os.path.join(HERE, d, "%s.framework" % framework))
            local("rsync -av %s %s" % (frameworkPath, d))

@task
def all():
    build("device")
    build("simulator")
    create_framework()
    copy_framework()

@task
def clean():
    build_path = "build-*%s" % (BUILD_SUFFIX,)
    local("rm -rf %s" % build_path)
    local("rm -rf %s" % FRAMEWORK_PATH)