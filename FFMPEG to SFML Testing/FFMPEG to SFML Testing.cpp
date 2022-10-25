
#include <string>
#include <windows.h>

void main()
{
    char cmd[] = "ffmpeg -f rawvideo -pix_fmt rgba -video_size 1920x1080 -r 30 -i pipe:0 -preset fast -y -pix_fmt yuv420p output.mp4";
    int* buffer = (int*)malloc(sizeof(int) *1920 * 1080);


    HANDLE readPipe;
    HANDLE writePipe;

    SECURITY_ATTRIBUTES sa = { sizeof(sa) };
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    CreatePipe(&readPipe, &writePipe, &sa, sizeof(buffer));

    SetHandleInformation(writePipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = readPipe;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION pi = {};

    CreateProcessA(nullptr, cmd, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);

    for (int frame = 0; frame < 1080; frame++)
    {
        for (int x = 0; x < 1920; x++)
        {
            buffer[frame * 1920 + x] = 0xffffff00 | (frame * 255 / 1079);
        }

        DWORD bytesWritten;
        WriteFile(writePipe, buffer, sizeof(buffer), &bytesWritten, nullptr);
    }

    CloseHandle(readPipe);
    CloseHandle(writePipe);
 }
