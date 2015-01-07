
for /l %%i in (1,1,20) do start cmd /c client_no_send.exe "127.0.0.1" 7000
	
for /l %%i in (1,1,3) do start cmd /c client.exe "127.0.0.1" 7100
