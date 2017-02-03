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

## Build Dependencies

 * OPC UA Ansi C Stack 1.03.340: https://github.com/OPCFoundation/UA-AnsiC
	   Please review official site page http://opcfoundation.github.io/UA-AnsiC
 * OpenSSL 1.0.2j: https://www.openssl.org/source/
 * Mdns (Multicast Domain Name System) library
 
## Runtime Dependencies

 * Bonjour Service (576.30.4)
 
## Building the LDS

VisualStudio 2013 solution is available for build.
Before building the LDS, the stack respective openssl must be already compiled for the desired target. Make sure that the proper submodule of the UaStack is extracted in the /stack folder.
For details on how to build the stack please refer to the ANSI C Stack readme file (https://github.com/OPCFoundation/UA-AnsiC).

## Package file structure description

The following tree shows the directory layout as required by the included project:

```
- /-- ualds
- |  |- AnsiCStack               OPC UA Ansi C Stack
- |    ||- core                  Configuration and utilities
- |    ||- platforms
- |      |||- linux              Platform adaption to OpenSSL and linux API
- |      |||- win32              Platform adaption to OpenSSL and Win32 API
- |    ||- proxystub
- |    ||- clientproxy           Client side top level API (optional)
- |    ||- serverstub            Server side top level API (optional)
- |    ||- securechannel         OPC UA secure conversation
- |    ||- stackcore             Base types and interfaces
- |    ||- transport
- |      |||- https              HTTPS transport (optional)
- |      |||- tcp                OPC TCP Binary transport
- |  |- mdns                     Multicast Domain Name System
- |  |- ualds                    Local Discovery Server
```