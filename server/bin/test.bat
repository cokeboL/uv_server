start cmd /c gate_server.exe "192.168.10.152" 7000
start cmd /c dispatch_log_server.exe "192.168.10.152" 7100
start cmd /c db_server.exe "192.168.10.152" 7200
start cmd /c bill_server.exe "192.168.10.152" 7300
start cmd /c logic_server.exe "192.168.10.152" 7401
rem start cmd /c logic_server.exe "127.0.0.1" 7402
rem start cmd /c logic_server.exe "127.0.0.1" 7403
