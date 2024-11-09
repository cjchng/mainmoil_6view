#include <iostream>
#include "apps/car6view/car6view.h"
#include "apps/measure/measure.h"
#include "apps/stitch180/stitch180.h"
#include "apps/fullmap/fullmap.h"
char getch(void);
int main(int argc, char *argv[])
{
// ./mainmoil senWidth senHeight iCx iCy Ratio imgWidth imgHeight caliRatio P0 P1 P2 P3 P4 P5
// ex. ./mainmoil 1.55 1.55 944 525 1 1920 1080 1   0 0 -20.772 35.661 -13.097 370.36    
if ( argc == 15 ) { // Generate X,Y Maps                
            
    double senWidth = atof(argv[1]); 
    double senHeight = atof(argv[2]); 
    int iCx = std::stoi (argv[3], nullptr, 0);
    int iCy = std::stoi (argv[4], nullptr, 0);
    double Ratio = atof(argv[5]); 
    int imgWidth = std::stoi (argv[6], nullptr, 0);
    int imgHeight = std::stoi (argv[7], nullptr, 0);
    double caliRatio = atof(argv[8]); 
    double P0 = atof(argv[9]); 
    double P1 = atof(argv[10]); 
    double P2 = atof(argv[11]); 
    double P3 = atof(argv[12]); 
    double P4 = atof(argv[13]); 
    double P5 = atof(argv[14]); 
    // std::cout << P5 << '\n';
    FullMap *f;
    f = new FullMap() ; 
    f->GenerateMaps(senWidth, senHeight, iCx, iCy, Ratio, imgWidth, imgHeight, caliRatio, 
        P0, P1, P2, P3, P4, P5);  
    delete f ;  


}
else  
{

    int opt;

    cout << endl << "----------------------------" << endl;
    cout << endl << "      MainMoil Menu   " << endl;
    cout << endl << "      1. Car - 6 channel views  " << endl;
    cout << endl << "      2. Measure " << endl;
    cout << endl << "      3. Stitch 180    " << endl;      
    cout << endl << "      4. FullMap    " << endl << endl;        
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
        case '4':
            FullMap *f;
            f = new FullMap() ; 
            f->Show();  
            delete f ;          
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
