@setlocal EnableDelayedExpansion
@for /f "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath') do @set vsPath=%%i
@set __VSCMD_ARG_NO_LOGO=1
@call "%vsPath%\VC\Auxiliary\Build\vcvarsall.bat" x64

if not exist build mkdir build
pushd build
del *.* /q

set debugCompilerFlags=/Od /Zi /DDEBUG
set releaseCompilerFlags=/O2 /GS-
set compilerFlags=/Fe:solutionist /FI"../source/forceInclude.h" %debugCompilerFlags%

set linkerFlags=/NODEFAULTLIB /SUBSYSTEM:CONSOLE /opt:ref /entry:wmain /MANIFEST:EMBED /MANIFESTINPUT:"../source/solutionist.manifest" kernel32.lib Bcrypt.lib

cl %compilerFlags% ..\source\*.cpp /link %linkerFlags%

popd