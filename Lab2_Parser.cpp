#include <iostream>
#include <libxml++-2.6/libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

int main(int argc, char* argv[]) {
    
    int sum = 0;
    std::string num_of_parking_space;
    std::string district, address;
    std::string filepath;

    if (argc > 1)   //define xml file
        filepath = argv[1];
    else
        filepath = "http://data.gov.spb.ru/opendata/7830001853-info_parking/data-20170704T155423-structure-20160328T153506.xml";

    xmlpp::TextReader reader(filepath);

    if (argc > 2)   //define district
        district = argv[2];
    else
        std::cin >> district;

    try {
        while(reader.read()) {
            int depth = reader.get_depth();
            if (depth == 2) {
                
                if ( ((std::string)reader.get_name())[1] == 'd' ) { // [1] == 'd' -> address
                    reader.read();
                    if (reader.has_value() && reader.get_depth() == 3) {
                            address = (std::string)reader.get_value();
                    }
                }

                else if ( ((std::string)reader.get_name())[0] == 'p' ) { // [0] = 'p' -> parking_space
                    reader.read();
                    if (reader.has_value() && reader.get_depth() == 3) {
                        num_of_parking_space = (std::string)reader.get_value();
                    }
                }

                else if ( ((std::string)reader.get_name())[0] == 'd' ) { // [0] = 'd' -> district_city
                    reader.read();
                    if(reader.has_value() && district == (std::string)reader.get_value() && reader.get_depth() == 3) {
                        std::cout << address << " == " << num_of_parking_space << " free parking spaces\n";
                        sum += atoi(num_of_parking_space.c_str());
                    }
                }
            }
            else
                continue;
        }
        
        std::cout << "Number of parking spaces: " << sum << '\n';
        }

        catch(const xmlpp::exception& ex)
        {
            std::cout << "libxml++ exception: " << ex.what() << std::endl;
        }
        return 0;
}
