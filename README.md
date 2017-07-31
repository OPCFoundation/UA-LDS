# UA-LDS

The OPC Foundation has formally released the OPC Unified Architecture Local Discovery Server to the community.

Please review official site page (https://OPCFoundation.github.io/UA-LDS/) for:
 * Overview
 * Licensing

Official installations for Windows are built by the OPC Foundation and provided at the OPC Foundation website (https://opcfoundation.org/).
 
## Contributing

We strongly encourage community participation and contribution to this project. First, please fork the repository and commit your changes there. Once happy with your changes you can generate a 'pull request'.

You must agree to the contributor license agreement before we can accept your changes. The CLA and "I AGREE" button is automatically displayed when you perform the pull request. You can preview CLA [here](https://opcfoundation.org/license/cla/ContributorLicenseAgreementv1.0.pdf).

OPC UA, empowering the Industrial Internet of Things (IIOT) and Industrie 4.0.

# Building and running the LDS

## Build Dependencies

 * OPC UA Ansi C Stack 1.03.340. (included) Please review official site page http://opcfoundation.github.io/UA-AnsiC.
 * CMake version 3.6.2 or later from https://cmake.org/install/.
 * Windows:
    - OpenSSL 1.0.2j (or later) -> either built from source or as pre-built package, for example from https://slproweb.com/products/Win32OpenSSL.html.
    - Bonjour Mdns/dnssd (Multicast Domain Name System) client library (included)
 * Linux:
    - OpenSSL 1.0.2j (or later)  -> (```sudo apt-get install libssl-dev```)  -> (```sudo yum install openssl-devel```)  
    - Avahi 0.61 (or later) libdnssd compatibility layer  -> (```sudo apt-get install libavahi-compat-libdnssd-dev```)  -> (```sudo yum install avahi-compat-libdns_sd-devel```)  
 
## Building the LDS

```bash
Make sure that openssl is installed or build. See UA-AnsiC build instructions.
Predefined scripts are available based on CMake. See build_win32.bat, build_win64.bat and build_linux.sh
```

## Runtime Dependencies

 * Windows: Bonjour Service (576.30.4)
 * Linux: Avahi Daemon (0.61 or later)

After a successful build binary files will be in <build-folder>\bin\[config] and should work as is.

## Package file structure description

The following tree shows the directory layout of this repo:

```
- /-- ualds                      LDS platform independent code
- |  |- stack                    OPC UA Ansi C Stack (https://github.com/OPCFoundation/UA-AnsiC)
- |    ||- stack
- |      |||- core                  Configuration and utilities
- |      |||- platforms
- |        ||||- linux              Platform adaption to OpenSSL and linux API
- |        ||||- win32              Platform adaption to OpenSSL and Win32 API
- |      |||- proxystub
- |      |||- clientproxy           Client side top level API (optional)
- |      |||- serverstub            Server side top level API (optional)
- |      |||- securechannel         OPC UA secure conversation
- |      |||- stackcore             Base types and interfaces
- |      |||- transport
- |        ||||- https              HTTPS transport (optional)
- |        ||||- tcp                OPC TCP Binary transport
- |  |- dnssd                    mdnsresponder (Bonjour) client library (Windows)
- |  |- win32                    Local Discovery Server Windows platform implementation
- |  |- linux                    Local Discovery Server Linux platform implementation
```