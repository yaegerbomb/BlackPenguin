#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <iostream>
#include "Base64.h"
#include <Windows.h>
#include "ImgurAPI.h"

using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;

string ClientID = "***REMOVED***";
string ClientSecret = "***REMOVED***";
string BaseURL = "https://api.imgur.com/3/";
string savedImagePath = "C:\\Users\\DYaeger\\Desktop\\blackpenguinsplash.png";

void UploadImgurImage(string *linkURL){
	//open file to upload
	ifstream imageToConvert;

	imageToConvert.open(savedImagePath, ios::binary);


	//declare our ostrm to hold our data
	ostringstream ostrm;
	if (imageToConvert.is_open())
	{
		ostrm << imageToConvert.rdbuf();
		imageToConvert.close();
	}

	const std::string s = ostrm.str();

	//encode string
	string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());

	//set up json object
	json::value obj;
	string_t  endcodedT = utility::conversions::to_string_t(encoded);
	obj[L"image"] = json::value::string(endcodedT);
	obj[L"type"] = json::value::string(U("base64"));

	//set up our client call
	string_t BaseURLT = utility::conversions::to_string_t(BaseURL + "upload");
	http_client client(BaseURLT);

	//set up our request
	http_request request(methods::POST);
	request.set_body(obj.serialize(), U("application/json"));

	//set up response and authroization heder
	http_response response(status_codes::OK);
	string_t ClientIDT = utility::conversions::to_string_t("Client-ID " + ClientID);
	request.headers().add(U("Authorization"), ClientIDT);


	//send request
	client.request(request).then([](http_response response)  -> pplx::task<json::value>
	{
		// Perform actions here to inspect the HTTP response...
		if (response.status_code() == status_codes::OK)
		{
			//if we were successful, extract json that was returned
			return response.extract_json();
		}

		// Handle error cases, for now return empty json value..
		return pplx::task_from_result(json::value());
	}).then([&linkURL](pplx::task<json::value> previousTask)
	{
		try
		{
			//get the json from previous task
			const json::value& v = previousTask.get();

			// Perform actions here to process the JSON value...
			//grab the link from the image model returned
			json::value imageModel = v.at(U("data"));
			*linkURL = ::utility::conversions::to_utf8string(imageModel.at(U("link")).as_string());
		}
		catch (const http_exception& e)
		{
			// Print error.
			wostringstream ss;
			ss << e.what() << endl;
			wcout << ss.str();
		}
	}).wait();
}

