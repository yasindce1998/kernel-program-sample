#include <ntddk.h>

#define IOCTL_CUSTOM_MESSAGE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS MyDriverIoctlHandler(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(Irp);

    if (irpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_CUSTOM_MESSAGE) {
        // Send a message to user mode (e.g., "Hello, world!")
        char* message = "Hello, world!";
        ULONG messageLength = (ULONG)strlen(message);

        Irp->IoStatus.Information = messageLength;
        status = STATUS_SUCCESS;

        if (Irp->MdlAddress) {
            void* buffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
            if (buffer) {
                RtlCopyMemory(buffer, message, messageLength);
            }
        }
    }

    return status;
}

