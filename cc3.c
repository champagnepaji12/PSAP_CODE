#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

int main(int argc, char* argv[]) {
    double amount;
    char from_currency[5], to_currency[5];
    double exchange_rate;
    char url[256];
    json_object *jobj, *jrate;
    CURL *curl;
    CURLcode res;

    // Get user input
    printf("Enter amount: ");
    scanf("%lf", &amount);
    printf("Enter currency to convert from (e.g. USD): ");
    scanf("%s", from_currency);
    printf("Enter currency to convert to (e.g. EUR): ");
    scanf("%s", to_currency);

    // Prepare the API request URL
    sprintf(url, "https://openexchangerates.org/api/latest.json?app_id=YOUR_APP_ID&base=%s&symbols=%s", from_currency, to_currency);

    // Initialize cURL
    curl = curl_easy_init();
    if(curl) {
        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Perform the request
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        } else {
            // Parse the JSON response
            jobj = json_tokener_parse(json_string);

            // Extract the exchange rate from the JSON object
            json_object_object_get_ex(jobj, "rates", &jrate);
            json_object_object_get_ex(jrate, to_currency, &jobj);
            exchange_rate = json_object_get_double(jobj);

            // Calculate and print the result
            double result = amount * exchange_rate;
            printf("%.4lf %s is equivalent to %.4lf %s\n", amount, from_currency, result, to_currency);
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    return 0;
}
