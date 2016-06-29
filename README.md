# UA-LDS
Local Discovery Server

More information about this project is available https://OPCFoundation.github.io/UA-LDS/

Before building the LDS the stack respective openssl must be already compiled for the desired target.
For details on how to build the stack please refer to the ANSI C Stack readme file.

The AnsiC stack needs special configuration for the LDS project.
in opcua_config.h, the following defines need these values:

	OPCUA_MULTITHREADED                        OPCUA_CONFIG_NO
	OPCUA_USE_SYNCHRONISATION                  OPCUA_CONFIG_NO
