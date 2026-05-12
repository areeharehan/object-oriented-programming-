#include <iostream>
using namespace std;
class Device{
    string Name;
    bool isON;
    int brightness;
    public:
        Device(){     // Default Constructor
            Name = "Unknown";
            isON = false;
            brightness = 50;
        }

        Device(string n, bool status, int b){  // Overloaded Constructor
            Name = n;
            isON = status;
            if (b >= 0 && b<= 100){   // Validation 
                brightness = b;
            }
            else{
                brightness = 50; // default value if invalid
            }
        }

        void setName(string n){
            Name = n;
        }
        string getName() const{
            return Name;
        }

        void setBrightness(int b){
            if (b >= 0 && b<= 100){   // Validation
                brightness = b;
            }
            else{
                cout << "Invalid brightness value. Must be 0-100." << endl;
            }
        }
        int getBrightness() const{
            return brightness;
        }

        void turnOn(){
            isON = true;
        }
        void turnOff(){
            isON = false;
        }

        bool getStatus() const{
            return isON;
        }

        void showInfo() const{
            cout << "Name of Device: " << Name
            << ", Status : " << (isON ? "ON" : "OFF") 
            << ", Brightness Level: " << brightness << endl;
        }

        ~Device(){    // Destructor
            cout<<"Device "<<Name<<" is being removed."<<endl;
        }
};

class Room{
    string roomName;
    Device devices[5];
    int count; // Number of devices currently in room

    public:
        Room(string name) : roomName(name) , count(0){}  //Constructor with member initializer list

        ~Room(){    // Destructor
            cout << "Room " << roomName << " manager closed." << endl;
        }
        
        // Adds device to array if space is available
        void addDevice(Device d){
            if (count < 5){
                devices[count] = d;
                count++;
                cout << "Device added to " << roomName << endl;
            }
            else{
                cout << "Room is full. Cannot add device." << endl;
            }
        }

        // Shows all devices in room
        void showAllDevices() const {
            if (count == 0) {
                cout << "No devices in " << roomName << endl;
                return;
            }

            cout << "Devices in " << roomName << ":" <<endl;

            for (int i = 0; i < count; i++){
                devices[i].showInfo();
            }
        }

        // Reurns index of device
        int findDevice(string name){
            for (int i = 0; i < count; i++){
                if (devices[i].getName() == name){
                    return i;
                }
            }
            return -1;
        }

        // Turns device ON/OFF
        void toggleDevice(string name){
            int index = findDevice(name);
            if (index == -1){
                cout << "Device not found." << endl;
                return;
            }
            if (devices[index].getStatus()){
                devices[index].turnOff();
            }
            else{
                devices[index].turnOn();
            }
            cout << "Device " << name << " is toggled." << endl;
        }


        // Changes Brightness
        void setDeviceBrightness(string name, int brightness){
            int index = findDevice(name);
            if (index == -1){
                cout << "Device not found." << endl;
                return;
            }
            devices[index].setBrightness(brightness);
            cout << "Brightness updated for " << name << endl;
        }
};

int main(){
    Room r("Living Room");

    int choice;

    do {
        cout << "\n=== Smart Room Manager ===" << endl;
        cout << "1. Add a Device" << endl;
        cout << "2. Show All Devices" << endl;
        cout << "3. Turn Device ON/OFF" << endl;
        cout << "4. Change Brightness" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        cin.ignore();  // clear newline in buffer so getline does not read an empty line

        switch(choice) {
            case 1:{
            string name;
            bool status;
            int brightness;

            cout << "Enter device name: ";
            getline(cin, name);

            cout << "Enter status (1 = ON, 0 = OFF): ";
            cin >> status;

            cout << "Enter brightness (0-100): ";
            cin >> brightness;

            Device d(name, status, brightness);
            r.addDevice(d);
            break;
            }

            case 2: {
            r.showAllDevices();
            break;
            }

            case 3: {
            string name;
            cout << "Enter device name to toggle: ";
            getline(cin >> ws, name);  // ws removes leadimg whitespace/newline
            r.toggleDevice(name);
            break;
            }

            case 4: {
            string name;
            int b;
            
            cout << "Enter device name: ";
            getline(cin >> ws, name);

            cout << "Enter new brightness (0-100): ";
            cin >> b;

            r.setDeviceBrightness(name, b);
            break;
            }

            case 5:
            cout << "Exiting Smart Room Manager ..." << endl;
            break;

            default:
            cout << "Invalid choice. Try again." <<  endl;
        }
    
} while (choice != 5);
return 0;
}
