del /s *.obj >nul 2>&1
del /s *.manifest >nul 2>&1
del /s *.ilk >nul 2>&1
del /s *.pdb >nul 2>&1
del /s *.exe >nul 2>&1
del /s *.dll >nul 2>&1
if exist output rd /s /q output >nul 2>&1
