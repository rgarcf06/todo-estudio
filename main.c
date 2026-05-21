#include "data.h"
#include "ui.h"

int main() {
    Data d;
    data_load(&d);
    run_ui(&d);
    return 0;
}
