#include "ClientTest.h"
#include <iostream>
#include <drogon/HttpClient.h>
#include <drogon/utils/Utilities.h> // For URL encoding if needed

void ClientTest::auth(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto client = HttpClient::newHttpClient("https://test.deribit.com");

    // Set up the query parameters as a query string
    std::string path = "/api/v2/public/auth";
    path += "?grant_type=client_credentials";
    path += "&client_id=client-id";
    path += "&client_secret=client-secret";

    // Create GET request
    auto request = HttpRequest::newHttpRequest();
    request->setMethod(drogon::Get);
    request->setPath(path);
    request->addHeader("Content-Type", "application/json");

    // Send the request asynchronously
    client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                        {
       auto response = HttpResponse::newHttpResponse();

       if (result == ReqResult::Ok && resp)
       {
           // Log the full JSON response body
           std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
           std::cout << "Response Body: " << resp->getBody() << std::endl;

           // Try to parse JSON response
           auto responseBody = resp->getJsonObject();
           if (responseBody)
           {
               std::cout << "Full JSON Response: " << responseBody->toStyledString() << std::endl;

               if (responseBody->isMember("result") && (*responseBody)["result"].isMember("access_token"))
               {
                   // Store the access token in the member variable
                   this->accessToken = (*responseBody)["result"]["access_token"].asString();
                   response->setBody("Access Token: " + this->accessToken);
               }
               else
               {
                   response->setBody("Failed to retrieve access token from response.");
               }
           }
           else
           {
               response->setBody("Response is not a valid JSON object.");
           }
       }
       else
       {
           // Log failure details
           std::cout << "Request failed with result: " << static_cast<int>(result) << std::endl;
           if (resp) {
               std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
               std::cout << "Error Response Body: " << resp->getBody() << std::endl;
           } else {
               std::cout << "Response was null." << std::endl;
           }

           response->setBody("Request to Deribit API failed.");
       }

       response->setStatusCode(k200OK);
       response->setContentTypeCode(CT_TEXT_PLAIN);
       callback(response); });
}
void ClientTest::performAuthenticatedRequest(std::function<void()> requestFunction)
{
    if (accessToken.empty())
    {
        auth(nullptr, [this, requestFunction](const HttpResponsePtr &resp)
             {
            // Retry the request after successful authentication
            if (!accessToken.empty())
            {
                requestFunction();
            } });
    }
    else
    {
        requestFunction();
    }
}

void ClientTest::placebuy(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                {
        auto client = HttpClient::newHttpClient("https://test.deribit.com");
        std::string path = "/api/v2/private/buy";
        path += "?amount=40";
        path += "&instrument_name=ETH-PERPETUAL";
        path += "&label=market0000235";
        path += "&type=market";

        auto request = HttpRequest::newHttpRequest();
        request->setMethod(drogon::Get);
        request->setPath(path);
        request->addHeader("Authorization", "Bearer " + accessToken);
        request->addHeader("Content-Type", "application/json");

        client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                            {
       auto response = HttpResponse::newHttpResponse();

       if (result == ReqResult::Ok && resp)
       {
           // Log the full JSON response body
           std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
           std::cout << "Response Body: " << resp->getBody() << std::endl;

           // Try to parse JSON response
           auto responseBody = resp->getJsonObject();
           if (responseBody)
           {
               std::cout << "Full JSON Response: " << responseBody->toStyledString() << std::endl;
           }
           else
           {
               response->setBody("Response is not a valid JSON object.");
           }
       }
       else
       {
           // Log failure details
           std::cout << "Request failed with result: " << static_cast<int>(result) << std::endl;
           if (resp) {
               std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
               std::cout << "Error Response Body: " << resp->getBody() << std::endl;
           } else {
               std::cout << "Response was null." << std::endl;
           }

           response->setBody("Request to Deribit API failed.");
       }

       response->setStatusCode(k200OK);
       response->setContentTypeCode(CT_TEXT_PLAIN);
       callback(response); }); });
}

void ClientTest::placesell(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                {
    auto client = HttpClient::newHttpClient("https://test.deribit.com");
    std::string path = "/api/v2/private/sell";
    path += "?amount=20";
    path += "&instrument_name=ETH-PERPETUAL";
    path += "&label=market0000235";
    path += "&type=market";

    auto request = HttpRequest::newHttpRequest();
    request->setMethod(drogon::Get);
    request->setPath(path);
    request->addHeader("Authorization", "Bearer " + accessToken);
    request->addHeader("Content-Type", "application/json");

    client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                        {
       auto response = HttpResponse::newHttpResponse();

       if (result == ReqResult::Ok && resp)
       {
           // Log the full JSON response body
           std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
           std::cout << "Response Body: " << resp->getBody() << std::endl;

           // Try to parse JSON response
           auto responseBody = resp->getJsonObject();
           if (responseBody)
           {
               std::cout << "Full JSON Response: " << responseBody->toStyledString() << std::endl;
           }
           else
           {
               response->setBody("Response is not a valid JSON object.");
           }
       }
       else
       {
           // Log failure details
           std::cout << "Request failed with result: " << static_cast<int>(result) << std::endl;
           if (resp) {
               std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
               std::cout << "Error Response Body: " << resp->getBody() << std::endl;
           } else {
               std::cout << "Response was null." << std::endl;
           }

           response->setBody("Request to Deribit API failed.");
       }

       response->setStatusCode(k200OK);
       response->setContentTypeCode(CT_TEXT_PLAIN);
       response->setBody(resp->getJsonObject()->toStyledString());
       callback(response); }); });
}

void ClientTest::placelimitbuy(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                {
    auto client = HttpClient::newHttpClient("https://test.deribit.com");
    std::string path = "/api/v2/private/buy";
    path += "?amount=1";
    path += "&instrument_name=ETH-PERPETUAL";
    path += "&label=thingsIdoForMoney";
    path += "&type=limit";
    path += "&price=5";

    auto request = HttpRequest::newHttpRequest();
    request->setMethod(drogon::Get);
    request->setPath(path);
    request->addHeader("Authorization", "Bearer " + accessToken);
    request->addHeader("Content-Type", "application/json");

    client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                        {
       auto response = HttpResponse::newHttpResponse();

       if (result == ReqResult::Ok && resp)
       {
           // Log the full JSON response body
           std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
           std::cout << "Response Body: " << resp->getBody() << std::endl;

           // Try to parse JSON response
           auto responseBody = resp->getJsonObject();
           if (responseBody)
           {
               std::cout << "Full JSON Response: " << responseBody->toStyledString() << std::endl;
           }
           else
           {
               response->setBody("Response is not a valid JSON object.");
           }
       }
       else
       {
           // Log failure details
           std::cout << "Request failed with result: " << static_cast<int>(result) << std::endl;
           if (resp) {
               std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
               std::cout << "Error Response Body: " << resp->getBody() << std::endl;
           } else {
               std::cout << "Response was null." << std::endl;
           }

           response->setBody("Request to Deribit API failed.");
       }

       response->setStatusCode(k200OK);
       response->setContentTypeCode(CT_TEXT_PLAIN);
       response->setBody(resp->getJsonObject()->toStyledString());
       callback(response); }); });
}

void ClientTest::placelimitsell(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                {
    auto client = HttpClient::newHttpClient("https://test.deribit.com");
    std::string path = "/api/v2/private/sell";
    path += "?amount=20";
    path += "&instrument_name=ALGO_USDC-PERPETUAL";
    path += "&label=market0000235";
    path += "&type=limit";
    path += "&price=0.2";

    auto request = HttpRequest::newHttpRequest();
    request->setMethod(drogon::Get);
    request->setPath(path);
    request->addHeader("Authorization", "Bearer " + accessToken);
    request->addHeader("Content-Type", "application/json");

    client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                        {
       auto response = HttpResponse::newHttpResponse();

       if (result == ReqResult::Ok && resp)
       {
           // Log the full JSON response body
           std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
           std::cout << "Response Body: " << resp->getBody() << std::endl;

           // Try to parse JSON response
           auto responseBody = resp->getJsonObject();
           if (responseBody)
           {
               std::cout << "Full JSON Response: " << responseBody->toStyledString() << std::endl;
           }
           else
           {
               response->setBody("Response is not a valid JSON object.");
           }
       }
       else
       {
           // Log failure details
           std::cout << "Request failed with result: " << static_cast<int>(result) << std::endl;
           if (resp) {
               std::cout << "Status Code: " << resp->getStatusCode() << std::endl;
               std::cout << "Error Response Body: " << resp->getBody() << std::endl;
           } else {
               std::cout << "Response was null." << std::endl;
           }

           response->setBody("Request to Deribit API failed.");
       }

       response->setStatusCode(k200OK);
       response->setContentTypeCode(CT_TEXT_PLAIN);
       response->setBody(resp->getJsonObject()->toStyledString());
       callback(response); }); });
}

void ClientTest::fetchOpenOrderIds(const HttpRequestPtr &req, std::function<void(const std::vector<std::string> &)> callback)
{
    auto client = HttpClient::newHttpClient("https://test.deribit.com");
    std::string path = "/api/v2/private/get_open_orders_by_instrument?instrument_name=ETH-PERPETUAL";

    auto request = HttpRequest::newHttpRequest();
    request->setMethod(drogon::Get);
    request->setPath(path);
    request->addHeader("Authorization", "Bearer " + accessToken);
    request->addHeader("Content-Type", "application/json");

    client->sendRequest(request, [callback](ReqResult result, const HttpResponsePtr &resp)
                        {
        std::vector<std::string> orderIds;
        
        if (result == ReqResult::Ok && resp)
        {
            auto responseBody = resp->getJsonObject();
            if (responseBody && responseBody->isMember("result"))
            {
                const auto &resultArray = (*responseBody)["result"];
                for (const auto &order : resultArray)
                {
                    orderIds.push_back(order["order_id"].asString());
                }
            }
        }
        callback(orderIds); });
}
void ClientTest::displayOrderIds(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                { fetchOpenOrderIds(req, [callback](const std::vector<std::string> &orderIds)
                                                    {
        auto response = HttpResponse::newHttpResponse();
        std::string htmlBody = "<html><body><h1>Order IDs:</h1><ul>";
        
        for (const auto &orderId : orderIds)
        {
            htmlBody += "<li>" + orderId + "</li>";
        }

        htmlBody += "</ul></body></html>";
        
        response->setStatusCode(k200OK);
        response->setContentTypeCode(CT_TEXT_HTML);
        response->setBody(htmlBody);

        callback(response); }); });
}

void ClientTest::modifyOrderQuantity(const std::string &orderId, int newQuantity, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto client = HttpClient::newHttpClient("https://test.deribit.com");
    std::string path = "/api/v2/private/edit";
    path += "?amount=" + std::to_string(newQuantity);
    path += "&order_id=" + orderId;
    auto request = HttpRequest::newHttpRequest();

    // Construct the JSON payload to modify the order quantity

    request->setMethod(drogon::Get);
    request->setPath(path);
    request->addHeader("Authorization", "Bearer " + accessToken);
    request->addHeader("Content-Type", "application/json");

    client->sendRequest(request, [callback](ReqResult result, const HttpResponsePtr &resp)
                        {
        auto response = HttpResponse::newHttpResponse();
        if (result == ReqResult::Ok && resp)
        {
            std::cout << "Modified Order Response: " << resp->getBody() << std::endl;
        }
        else
        {
            std::cout << "Failed to modify order with ID: " << resp->getBody() << std::endl;
        }
        response->setBody(resp->getJsonObject()->toStyledString());
        callback(resp); });
}
void ClientTest::modifyOrder(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                { fetchOpenOrderIds(req, [this, callback = std::move(callback)](const std::vector<std::string> &orderIds)
                                                    {
        for (const auto &orderId : orderIds)
        {
            modifyOrderQuantity(orderId, 1, [callback](const HttpResponsePtr &resp)
            {
                // You can call the original callback here if needed
                callback(resp);
            });
        } }); });
}
void ClientTest::cancelOrder(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                { fetchOpenOrderIds(req, [this, callback = std::move(callback)](const std::vector<std::string> &orderIds)
                                                    {
                    int count = 0;
        for (const auto &orderId : orderIds)
        {
            count++;
            if(count == 3) break;
            std::cout<<orderId<<std::endl;
            auto client = HttpClient::newHttpClient("https://test.deribit.com");
            std::string path = "/api/v2/private/cancel";
            path += "?order_id="+ orderId;

            auto request = HttpRequest::newHttpRequest();

    // Construct the JSON payload to modify the order quantity

            request->setMethod(drogon::Get);
            request->setPath(path);
            request->addHeader("Authorization", "Bearer " + accessToken);
            request->addHeader("Content-Type", "application/json");

            client->sendRequest(request, [callback](ReqResult result, const HttpResponsePtr &resp)
                                {
                auto response = HttpResponse::newHttpResponse();
                if (result == ReqResult::Ok && resp)
                {
                    std::cout << "Cancel Order Response: " << resp->getBody() << std::endl;
                }
                else
                {
                    std::cout << "Failed to cancel order with ID: " << resp->getBody() << std::endl;
                }
                response->setBody(resp->getJsonObject()->toStyledString());
                callback(resp); });


        } }); });
}
void ClientTest::currentPositions(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                {
        auto client = HttpClient::newHttpClient("https://test.deribit.com");
        std::string path = "/api/v2/private/get_positions";
        path += "?currency=ETH"; // Modify currency if needed
        path += "&kind=future";  // Adjust to "option" if querying options positions

        auto request = HttpRequest::newHttpRequest();
        request->setMethod(drogon::Get);
        request->setPath(path);
        request->addHeader("Authorization", "Bearer " + accessToken);
        request->addHeader("Content-Type", "application/json");

        client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                            {
            auto response = HttpResponse::newHttpResponse();
            
            if (result == ReqResult::Ok && resp)
            {
                // Parse JSON response
                auto responseBody = resp->getJsonObject();
                if (responseBody && responseBody->isMember("result"))
                {
                    std::cout << "Current Positions: " << responseBody->toStyledString() << std::endl;
                    response->setBody(responseBody->toStyledString());
                }
                else
                {
                    std::cout << "No open positions or error in parsing JSON response." << std::endl;
                    response->setBody("No open positions or invalid JSON response.");
                }
            }
            else
            {
                // Handle request failure
                std::cout << "Request to fetch current positions failed. Status code: " 
                          << (resp ? resp->getStatusCode() : 0) << std::endl;
                response->setBody("Failed to fetch current positions.");
            }

            response->setStatusCode(k200OK);
            response->setContentTypeCode(CT_TEXT_PLAIN);
            callback(response); 
        }); });
}
void ClientTest::getOrderBook(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    performAuthenticatedRequest([this, req, callback]()
                                {
        auto client = HttpClient::newHttpClient("https://test.deribit.com");
        std::string path = "/api/v2/public/get_order_book";
        path += "?instrument_name=ETH-PERPETUAL";

        auto request = HttpRequest::newHttpRequest();
        request->setMethod(drogon::Get);
        request->setPath(path);
        request->addHeader("Content-Type", "application/json");

        client->sendRequest(request, [this, callback](ReqResult result, const HttpResponsePtr &resp)
                            {
            auto response = HttpResponse::newHttpResponse();

            if (result == ReqResult::Ok && resp)
            {
                // Parse JSON response
                auto responseBody = resp->getJsonObject();
                if (responseBody && responseBody->isMember("result"))
                {
                    std::cout << "Order Book: " << responseBody->toStyledString() << std::endl;
                    response->setBody(responseBody->toStyledString());
                }
                else
                {
                    std::cout << "Failed to parse order book JSON response." << std::endl;
                    response->setBody("Failed to retrieve order book data.");
                }
            }
            else
            {
                // Handle request failure
                std::cout << "Request to fetch order book failed. Status code: " 
                          << (resp ? resp->getStatusCode() : 0) << std::endl;
                response->setBody("Failed to fetch order book.");
            }

            response->setStatusCode(k200OK);
            response->setContentTypeCode(CT_TEXT_PLAIN);
            callback(response); 
        }); });
}

void ClientTest::console(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto response = HttpResponse::newHttpResponse();
    response->setStatusCode(k200OK);
    response->setContentTypeCode(CT_TEXT_HTML);
    response->setBody("testing");
    callback(response);
}
