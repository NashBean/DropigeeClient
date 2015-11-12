//
//  DropigeeClientAPI.h
//  Dropigee
//
//  Created by nash on 11/12/15.
//  Copyright 2015 iBean Software. 
//  All rights reserved.
//

#ifndef Dropigee_Client_API_h
#define Dropigee_Client_API_h

#include <vector>
#include <string>
#include <sstream>
//#include "oauth2cpp/TokenInfo.hpp"
#include "oauth2cpp/DefaultStorage.hpp"



const int DCAPI_MAJOR_VERSION = 0;
const int DCAPI_MINOR_VERSION = 2;


namespace dropigee 
{

# define APIcom "api.dropigee.com"

namespace {
    enum {
        REQUEST_TYPE = QNetworkRequest::User
    };
    
    enum{
        REQUEST_TOKEN,
        REQUEST_REFRESH,
        REQUEST_API
    };
}


struct accept_header
{//Accept: application/vnd.dropigee.v1+json
    std::stringstream result; 
};

//  Blank fields are included as null instead of being omitted.
//  All timestamps are returned in ISO 8601 format:
//  YYYY-MM-DDTHH:MM:SSZ
enum Client_Errors
{
    ce_NoError = 0,
    ce_Unauthorized = 401,  //   401 Unauthorized.
    ce_Unprocessable = 422 
    //Sending invalid fields will result in a - 422 Unprocessable Entity response.
};

//OAuth2 Token (sent in a header)
//curl -H "Authorization: Bearer OAUTH-TOKEN" https://api.dropigee.com/
//OAuth2 Token (sent as a parameter)
//curl https://api.dropigee.com/?access_token=OAUTH-TOKEN     

//***********   OAuth2   *******************
//--------Web Application Flow--------------
//This is a description of the OAuth2 flow from 3rd party web sites.

//(1.) Redirect users to request Dropigee access

//GET https://www.dropigee.com/oauth/authorize/

struct access_parameters 
{
    std::string client_id;//Required. The client_id you received from Dropigee when you registered.
    std::string response_type; 	//Required. (e.g. code)
    std::string redirect_uri; //The URL in your app where users will be sent after authorization.
    std::string scope; 	//A list of scopes separated by spaces. If not provided, scope defaults to an empty list of scopes for users that don’t have a valid token for the app.
    std::string state; 	// protection string.
};
    class DropigeeToken : public OAUTH2CPP_API BearerToken 
    {
    public:
        BearerToken();
        ~BearerToken();
        
        // TokenInfo interface
        std::string getAccessToken() const override;
        std::string getRefreshToken() const override;
        std::string getType() const override;
        uint32_t expiresIn() const override;
        void setAccessToken(const std::string &value) override;
        void setRefreshToken(const std::string &value) override;
        void setType(const std::string &value) override {}
        void expiresIn(uint32_t value) override;
        bool hasAccessToken() override;
        bool hasRefreshToken() override;
        
    private:
        std::string accessToken;
        std::string refreshToken;
        uint32_t expireValue;
    };


//(2.) Dropigee redirects back to your site

//If the user accepts your request, Dropigee redirects back to your site with a temporary code in a code parameter as well as the state you provided in the previous step in a state parameter. If the states don’t match, the request has been created by a third party and the process should be aborted.

//Exchange this for an access token:

//  POST https://www.dropigee.com/oauth/token/
struct web_access_token
{
    std::string client_id;	//Required. The client_id you received from Dropigee when you registered.
    std::string client_secret;	//Required. The client_secret you received from Dropigee when you registered.
    std::string code;	//Required. The code you received as a response to Step 1.
    std::string grant_type;	//Required. (e.g. authorization_code)
    std::string redirect_uri;	//The URL in your app where users will be sent after authorization.
    std::string scope; //A list of scopes separated by spaces. If not provided, scope defaults to an empty list of scopes for users that don’t have a valid token for the app.
    std::string state;	//A protection string.
};


/*  // ----------   Response:
 {
 "access_token": "3f44b238856fe8e620ee55121ec99828d7be89c38fb76f264d77670f3c2466ed",
 "token_type": "bearer",
 "scope": "read",
 "created_at": 1446839498
 }
 3. Use the access token to access the API
 The access token allows you to make requests to the API on a behalf of a user.
 */

//--------  Non-Web Application Flow --------------
//This is a description of the OAuth2 flow when the client is requesting access to protected resources under its control (i.e. there is no third party).

//(1). Request an access token from Dropigee

//  POST https://www.dropigee.com/oauth/token/

/* use web_access_token struct for both
struct non_web_access_token
{   //Parameters:
    std::string client_id;  //Required. The client_id you received from Dropigee when you registered.
    std::string client_secret; //Required. The client_secret you received from Dropigee when you registered.
    std::string grant_type; //Required. (e.g. client_credentials)
    std::string scope; //A list of scopes separated by spaces. If not provided, scope defaults to an empty list of scopes for users that don’t have a valid token for the app.
};
//*/

/*--------      Response:
 
 {
 "access_token": "3f44b238856fe8e620ee55121ec99828d7be89c38fb76f264d77670f3c2466ed",
 "token_type": "bearer",
 "scope": "read",
 "created_at": 1446839498
 }
 
 2. Use the access token to access the API
 
 The access token allows you to make requests to the API on a behalf of the client.
 
*/

    class DCAPI 
    {
        web_access_token wat;
    public:
        bool non_web;
    };

    class DropigeeClient : private DCAPI
    {
    public:
    };
    
//* rem switch   
    class Dropigee_API: public OAUTH2CPP_API 
    {
    public:
        virtual HttpRequest* createRequest() const = 0;
        virtual void get(HttpRequest* request) = 0;
        virtual void post(HttpRequest* request, const std::string& data) = 0;
    };
    
/*/

    class Dropigee_API: public OAUTH2CPP_API
    {
    public:
        std::string getAccessToken() const = 0;
        std::string getRefreshToken() const = 0;
        std::string getType() const = 0;
        uint32_t expiresIn() const = 0;
        
        void setAccessToken(const std::string& value) = 0;
        void setRefreshToken(const std::string& value) = 0;
        void setType(const std::string& value) = 0;
        void expiresIn(uint32_t value) = 0;
        
        bool hasAccessToken() = 0;
        bool hasRefreshToken() = 0;
        
        ~TokenInfo() = default;
        
    private:
    };
//*/        
}// end of namespace dropigee 

#endif // Dropigee_Client_API_h

