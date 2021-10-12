#include "../util/tools.h"
#include "demo.h"

#include <stdlib.h>

// for current_path()
// #include <filesystem>   C++17
#include <unistd.h>

//extern struct cmr_img_stich_record_camraLR_on_off a_cj_xyz;
extern running_mode_t el_state;


int main(int argc, char *argv[])
{
    #include <fstream>
    using std::ofstream;

    for(int i=0 ; i<argc ; i++)
        cout << "Argument: " << argv[i] << endl ;
    char *cwd = get_current_dir_name();
    cout << cwd << endl ;
    string ing_script(now_as_string());
    ing_script.append(".symlnk") ;

    string ing_msg(now_as_string()) ;
    ing_msg.append(".msg") ;

    ofstream outdata ;
    outdata.open(ing_msg) ;

    string set_wrk_symbolic_link("ln -s ");
    set_wrk_symbolic_link.append(argv[1]) ;
    set_wrk_symbolic_link.append(" ") ;
    set_wrk_symbolic_link.append(ing_script) ;
    outdata << argv[1] ;
    //set_wrk_symbolic_link.append("run-") ;
    //set_wrk_symbolic_link.append(argv[1]) ;
    //set_wrk_symbolic_link.append("@") ;
    //set_wrk_symbolic_link.append(now_as_string()) ;
    //cout << set_wrk_symbolic_link << endl ;
    system(set_wrk_symbolic_link.c_str()) ;
    //cout << current_path() << endl ;
    outdata.close() ;
    exit(0) ;

    std::ifstream in(argv[1]);
    std::cin.rdbuf(in.rdbuf());

    el_state.util_media_type = Util::MediaType::IMAGE_FILE;
    el_state.util_system_state = Util::SystemState::NPPM; //CAR_VIEW ; //
    el_state.recordState = Util::RecordOnOff::OFF;

    char buf[256];
    cin2buf(buf, 128); //first line of running recipe

    XAnypoint *m;
    m = new XAnypoint();
    m->run();
    delete m;
}
