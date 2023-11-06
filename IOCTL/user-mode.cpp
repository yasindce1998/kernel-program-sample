#include <Windows.h>
#include <iostream>

int main()
{
    HANDLE hDevice = CreateFile(L"\\\\.\\MyDriverName", GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open the driver." << std::endl;
        return 1;
    }

    char message[100];
    DWORD bytesReturned;

    if (DeviceIoControl(hDevice, IOCTL_CUSTOM_MESSAGE, nullptr, 0, message, sizeof(message), &bytesReturned, nullptr)) {
        std::cout << "Received message from kernel: " << message << std::endl;
    } else {
        std::cerr << "DeviceIoControl failed." << std::endl;
    }

    CloseHandle(hDevice);

    return 0;
}
