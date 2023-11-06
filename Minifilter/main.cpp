#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

PFLT_FILTER gFilterHandle;

FLT_PREOP_CALLBACK_STATUS MyPreOperationCallback(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Out_ PVOID* CompletionContext
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    if (Data->Iopb->MajorFunction == IRP_MJ_CREATE || Data->Iopb->MajorFunction == IRP_MJ_READ) {
        UNICODE_STRING fileName = Data->Iopb->TargetFileObject->FileName;
        DbgPrint("File Accessed: %wZ\n", &fileName);
    }

    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status;

    status = FltRegisterFilter(DriverObject, &FilterRegistration, &gFilterHandle);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = FltStartFiltering(gFilterHandle);
    if (!NT_SUCCESS(status)) {
        FltUnregisterFilter(gFilterHandle);
        return status;
    }

    return STATUS_SUCCESS;
}

NTSTATUS DriverUnload(
    _In_ FLT_FILTER_UNLOAD_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(Flags);

    FltUnregisterFilter(gFilterHandle);

    return STATUS_SUCCESS;
}
