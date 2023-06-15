# **03.01.WinInet-download**

## **OBJECTIVES**

* Understand how to make HTTP Requests using WinInet.

## **STEPS**

1). Download `1MB.zip` and `5MB.zip` files, and save them in the current folder.

* http://ipv4.download.thinkbroadband.com/1MB.zip
* http://ipv4.download.thinkbroadband.com/5MB.zip

```bat
f00be31bb73fe783209497d80aa1de89  1MB.zip
b3215c06647bc550406a9c8ccc378756  5MB.zip
```

MD5 file:

```bat
CertUtil -hashfile <path to file> MD5
```

Start a web server on 127.0.0.1 port 9090

```bat
py -m http.server -b 127.0.0.1 9090
```

2). Implement DownloadFile.exe

Resolve the items in main.cpp:

* TODO 1 InternetOpenA
* TODO 2 InternetConnectA
* TODO 3 HttpOpenRequestA

3). Use `DownloadFile.exe` to download both zips from your local python server. Save them as `1MB-new.zip` and `5MB-new.zip`.

Verify the output using `certutil` afterwards.

* `certutil -hashfile 1MB.zip MD5` compare against `certutil -hashfile 1MB-new.zip MD5`
* `certutil -hashfile 5MB.zip MD5` compare against `certutil -hashfile 5MB-new.zip MD5`

## REFERENCES

* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopena
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetconnecta
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-httpopenrequesta
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetsetoptiona
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-httpsendrequesta
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetreadfile
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetclosehandle
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-httpqueryinfoa
* https://learn.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-internetopenurla
 