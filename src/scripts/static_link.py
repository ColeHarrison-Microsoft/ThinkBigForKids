#
# PlatformIO extra script: statically link the C/C++ runtimes for the native
# (simulator) build so the produced binary runs on its own, without requiring
# the MinGW/GCC runtime DLLs (libstdc++-6.dll, libgcc_s_seh-1.dll,
# libwinpthread-1.dll) to be on PATH.
#
# build_flags does not reliably reach the link step for these driver flags, so
# we append them to LINKFLAGS explicitly here.
#
Import("env")

# Only the native (simulator) build benefits from statically linking the host
# C/C++ runtimes. On the AVR backends (mBot, Arduino car) these driver flags are
# meaningless or harmful, so this script is a no-op there.
if env.get("PIOPLATFORM") == "native":
    env.Append(
        LINKFLAGS=[
            "-static",
            "-static-libgcc",
            "-static-libstdc++",
        ]
    )
