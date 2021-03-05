#ifndef MOVIES_MICROSERVICES_WEATHERHANDLER_H
#define MOVIES_MICROSERVICES_WEATHERHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/WeatherService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class WeatherServiceHandler : public WeatherServiceIf {
 public:
  WeatherServiceHandler();
  ~WeatherServiceHandler() override=default;

  WeatherType::type GetWeather(const int64_t city) override;
};

// Constructor
WeatherServiceHandler::WeatherServiceHandler() {

}

// Remote Procedure "PlaceOrder"

WeatherType::type WeatherServiceHandler::GetWeather(const int64_t city) {
     // Your implementation goes here
     printf("GetWeather\n");

    // randomly select a weather
    return (0 == rand()%2)? WeatherType::type::COLD : WeatherType::type::WARM;
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_WEATHERHANDLER_H

