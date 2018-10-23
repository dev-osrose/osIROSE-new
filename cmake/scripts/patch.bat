@echo off
REM try a revert first
"@Patch_EXECUTABLE@" -R --dry-run -p0 -N -i %1
if errorlevel 1 (
  "@Patch_EXECUTABLE@" -p0 -N -i %1
)
REM else we are lucky, patch was already applied and we can ignore everything
