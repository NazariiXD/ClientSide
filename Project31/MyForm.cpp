#include "MyForm.h"

using namespace Project31;
[STAThread]
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(0);

    Project31::MyForm form;
    Application::Run(%form);

    return 0;
}

