#include "apps/car6view/car6view.h"
#include "apps/measure/measure.h"
#include "apps/stitch180/stitch180.h"
char getch(void);
int main(int argc, char *argv[])
{
                
  // while(1) 
{

    int opt;

    cout << endl << "----------------------------" << endl;
    cout << endl << "      MainMoil Menu   " << endl;
    cout << endl << "      1. Car - 6 channel views  " << endl;
    cout << endl << "      2. Measure " << endl;
    cout << endl << "      3. Stitch 180    " << endl << endl;      
    cout << endl << "      Esc to Exit    " << endl;    
    cout << endl << "----------------------------" << endl;

    char c;
    // while(1){ 
    c = getch();
cout << c << endl;
    switch (c) 
    {
        case '1':
            Car6view *w;
            w = new Car6view() ; 
            w->Show();  
            delete w ;          
            break;
        case '2':
            Measure *m;
            m = new Measure() ; 
            m->Show();  
            delete m ;                     
            break;
        case '3':
            Stitch180 *s;
            s = new Stitch180() ; 
            s->Show();  
            delete s ;                     
            break;            
        case 27:
            exit(1);
            break;
        default:
            break;
    }

    // }
}
}
char getch (void)
{
    int ch;
    struct termios oldt, newt;
 
    tcgetattr(STDIN_FILENO, &oldt);
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~( ECHO | ICANON | ECHOE | ECHOK |
                       ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
 
    return ch;
}