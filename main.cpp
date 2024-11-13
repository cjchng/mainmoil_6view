#include <iostream>
#include "apps/car6view/car6view.h"
#include "apps/measure/measure.h"
#include "apps/stitch180/stitch180.h"
#include "apps/fullmap/fullmap.h"
char getch(void);
int main(int argc, char *argv[])
{
    double senWidth, senHeight, Ratio, caliRatio, P0, P1, P2, P3, P4, P5;
    int iCx, iCy, imgWidth, imgHeight, Mode;
    double Alpha[4], Beta[4], Zoom[4], alphaMax; 
 
//(16) ./mainmoil EQUIRECTANGULAR senWidth senHeight iCx iCy Ratio imgWidth imgHeight caliRatio P0 P1 P2 P3 P4 P5
//(20) ./mainmoil ANYPOINT senWidth senHeight iCx iCy Ratio imgWidth imgHeight caliRatio P0 P1 P2 P3 P4 P5 Mode Alpha Beta Zoom 
//(17) ./mainmoil PANORAMA senWidth senHeight iCx iCy Ratio imgWidth imgHeight caliRatio P0 P1 P2 P3 P4 P5 alphaMax
//(29) ./mainmoil ANYPOINT senWidth senHeight iCx iCy Ratio imgWidth imgHeight caliRatio P0 P1 P2 P3 P4 P5 Mode Alpha1 Beta1 Zoom1 Alpha2 Beta2 Zoom2 Alpha3 Beta3 Zoom3 Alpha4 Beta4 Zoom4   
// Examples
// 1. ./mainmoil EQUIRECTANGULAR 1.55 1.55 944 525 1 1920 1080 1   0 0 -20.772 35.661 -13.097 370.36    
// 2. ./mainmoil ANYPOINT 1.55 1.55 944 525 1 1920 1080 1   0 0 -20.772 35.661 -13.097 370.36  1   0  0  6    
// 3. ./mainmoil PANORAMA 1.55 1.55 944 525 1 1920 1080 1   0 0 -20.772 35.661 -13.097 370.36  110
// 4. ./mainmoil ANYPOINT_QUAD 1.55 1.55 944 525 1 1920 1080 1   0 0 -20.772 35.661 -13.097 370.36  1   0  0  6  0 -70  6  0  70  6 -45  45  6    

if (( argc == 16 )||( argc == 17 )||( argc == 20 )||( argc == 29 )) { // Generate X,Y Maps                
    FullMap *f;
    f = new FullMap() ; 
    f->cmdType = CmdType::NONE;
 // convert argv[1] to lowercase
 for (int i = 0; i < strlen(argv[1]); i++) {
    argv[1][i] = tolower(argv[1][i]);
  }

    if (( argc == 16 ) && (strcmp( argv[1], "equirectangular") == 0)) { 
        f->cmdType = CmdType::EQUIRECTANGULAR;
    } else if (( argc == 20 ) && (strcmp( argv[1], "anypoint") == 0)) { 
        f->cmdType = CmdType::ANYPOINT;
    } else if (( argc == 17 ) && (strcmp( argv[1], "panorama") == 0)) { 
        f->cmdType = CmdType::PANORAMA;    
    } else if (( argc == 29 ) && (strcmp( argv[1], "anypoint_quad") == 0)) { 
        f->cmdType = CmdType::ANYPOINT_QUAD;
    }    
if ( f->cmdType != CmdType::NONE ) {
    int idx = 2;
    // Common Parameters
    senWidth = atof(argv[idx++]); 
    senHeight = atof(argv[idx++]); 
    iCx = stoi (argv[idx++], nullptr, 0);
    iCy = stoi (argv[idx++], nullptr, 0);
    Ratio = atof(argv[idx++]); 
    imgWidth = stoi (argv[idx++], nullptr, 0);
    imgHeight = stoi (argv[idx++], nullptr, 0);
    caliRatio = atof(argv[idx++]); 
    P0 = atof(argv[idx++]); 
    P1 = atof(argv[idx++]); 
    P2 = atof(argv[idx++]); 
    P3 = atof(argv[idx++]); 
    P4 = atof(argv[idx++]); 
    P5 = atof(argv[idx++]); 

if ( f->cmdType == CmdType::ANYPOINT ) {
    Mode = stoi (argv[idx++], nullptr, 0);  
    Alpha[0] = atof(argv[idx++]); 
    Beta[0] = atof(argv[idx++]); 
    Zoom[0] = atof(argv[idx++]); 
}
else if ( f->cmdType == CmdType::PANORAMA ) {
    alphaMax = atof(argv[idx++]); 
}
else if ( f->cmdType == CmdType::ANYPOINT_QUAD ) {
    Mode = stoi (argv[idx++], nullptr, 0);  
    for ( int i = 0 ; i < 4 ; i++ ) {
        Alpha[i] = atof(argv[idx++]); 
        Beta[i] = atof(argv[idx++]); 
        Zoom[i] = atof(argv[idx++]);
    } 
}

    // std::cout << P5 << '\n';
    switch ( f->cmdType ){
        case CmdType::EQUIRECTANGULAR :
    f->GenerateMaps_Equi(senWidth, senHeight, iCx, iCy, Ratio, imgWidth, imgHeight, caliRatio, 
        P0, P1, P2, P3, P4, P5);
        break;
        case CmdType::ANYPOINT :
    f->GenerateMaps_Anypoint(senWidth, senHeight, iCx, iCy, Ratio, imgWidth, imgHeight, caliRatio, 
        P0, P1, P2, P3, P4, P5, Mode, Alpha[0], Beta[0], Zoom[0] );
        break;
        case CmdType::ANYPOINT_QUAD :
    f->GenerateMaps_AnypointQuad(senWidth, senHeight, iCx, iCy, Ratio, imgWidth, imgHeight, caliRatio, 
        P0, P1, P2, P3, P4, P5, Mode, Alpha, Beta, Zoom );
        break;           
        case CmdType::PANORAMA :
    f->GenerateMaps_Panorama(senWidth, senHeight, iCx, iCy, Ratio, imgWidth, imgHeight, caliRatio, 
        P0, P1, P2, P3, P4, P5, alphaMax);
        break;
     
    }
}
else
   cout << endl << "Parameter Error - (" << argc << "," << argv[1] << ")" <<  endl;

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
