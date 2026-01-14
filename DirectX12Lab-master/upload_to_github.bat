@echo off
chcp 65001 >nul
git config core.quotepath off

echo ========================================
echo Загрузка кода на GitHub
echo ========================================
echo.

echo Добавление всех изменений...
git add -A

echo.
echo Создание коммита...
git commit -m "Added Sponza model loading support"

echo.
echo Отправка на GitHub...
git push origin main

echo.
echo ========================================
echo Готово!
echo ========================================
pause

