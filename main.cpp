
using namespace std;

//Setup for serial comms
enum state_codes {    wait, get_sof1, check_get_sof2, check_get_dest, check_get_src, get_size, setup_count, get_byte_dec, get_parity, get_eof, check_parity, write_data, discard_data } current_state, next_state;

enum event_code { no_event, byte_received, error, timeout } current_event;

//Sets the current event
void event_set(enum event_code new_event){
    current_event=new_event;
}

//Clears current event
void event_clear(){
    current_event=no_event;
}

//Returns current event
enum event_code event_get(){
    return current_event;
}

//Checks for an incoming byte_recieved
void waitFunc()
{
    switch(event_get())
    {
        case byte_received:
            next_state=get_sof1;
            break;

        case no_event:
            next_state=wait;
            break;

        case error:
            next_state=wait;
            break;

        case timeout:
            next_state=wait;
            break;
    }
}

//Handles states and their behaviour
void stateMachine()
{
    switch(current_state)
    {
    case wait:
        waitFunc();
        break;

    case get_sof1:

        break;
    case check_get_sof2:

        break;
    case check_get_dest:

        break;
    case check_get_src:

        break;
    case get_size:

        break;
    case setup_count:

        break;
    case get_byte_dec:

        break;
    case get_parity:

        break;
    case get_eof:

        break;
    case check_parity:

        break;
    case write_data:

        break;
    case discard_data:

        break;
    default:
        break;
    }
}

void setup()
{
    return;
}

void loop()
{
    return;
}