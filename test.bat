@echo off
cd /D "D:\24_huawei_com\WindowsRelease"
for /R "D:\24_huawei_com\WindowsRelease\maps" %%F in (*) do (
   echo start
   echo.
   <NUL set /p "=%%~nF : "
   .\PreliminaryJudge_win_1.exe -m "%%F" "C:\Users\15461\Desktop\24_huawei\cmake-build-debug\24_huawei.exe" -l NONE  
   echo end
   echo.
)
echo all tested
pause
