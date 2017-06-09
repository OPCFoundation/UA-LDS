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

 * OPC UA Ansi C Stack 1.03.340. (included) Please review official site page http://opcfoundation.github.io/UA-AnsiC
 * CMake version 3.6.2 or later.
 * OpenSSL 1.0.2j: https://www.openssl.org/source/
 * Windows:
    - Bonjour Mdns/dnssd (Multicast Domain Name System) client library (included)
 * Linux:
    - Avahi-client library 0.61 or later (e.g. through libavahi-compat-libdnssd-dev package)
 
## Building the LDS

```bash
- mkdir <build-folder>
- cd <build-folder>
- cmake ..
- cmake --build .
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