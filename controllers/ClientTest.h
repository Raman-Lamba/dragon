#pragma once

#include <drogon/HttpController.h>
#include <iostream>
using namespace drogon;

class ClientTest : public drogon::HttpController<ClientTest>
{
public:
  METHOD_LIST_BEGIN
  // use METHOD_ADD to add your custom processing function here;
  // METHOD_ADD(ClientTest::get, "/{2}/{1}", Get); // path is /ClientTest/{arg2}/{arg1}
  // METHOD_ADD(ClientTest::your_method_name, "/{1}/{2}/list", Get); // path is /ClientTest/{arg1}/{arg2}/list
  // ADD_METHOD_TO(ClientTest::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
  ADD_METHOD_TO(ClientTest::auth, "/auth", drogon::Get);
  ADD_METHOD_TO(ClientTest::placebuy, "/placebuy", drogon::Get);
  ADD_METHOD_TO(ClientTest::placesell, "/placesell", drogon::Get);
  ADD_METHOD_TO(ClientTest::placelimitbuy, "/placelimitbuy", drogon::Get);
  ADD_METHOD_TO(ClientTest::placelimitsell, "/placelimitsell", drogon::Get);
  ADD_METHOD_TO(ClientTest::console, "/console", drogon::Get);
  ADD_METHOD_TO(ClientTest::modifyOrder, "/modifyOrder", drogon::Get);
  ADD_METHOD_TO(ClientTest::cancelOrder, "/cancelOrder", drogon::Get);
  ADD_METHOD_TO(ClientTest::currentPositions, "/currentPositions", drogon::Get);
  ADD_METHOD_TO(ClientTest::getOrderBook, "/getOrderBook", drogon::Get);
  ADD_METHOD_TO(ClientTest::displayOrderIds, "/displayOrderIds", drogon::Get);

  METHOD_LIST_END
  // your declaration of processing function maybe like this:
  // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
  // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
  void auth(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void placebuy(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void placesell(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void placelimitbuy(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void placelimitsell(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void modifyOrder(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void cancelOrder(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void fetchOpenOrderIds(const HttpRequestPtr &req, std::function<void(const std::vector<std::string> &)> callback);
  void modifyOrderQuantity(const std::string &orderId, int newQuantity, std::function<void(const HttpResponsePtr &)> &&callback);
  void currentPositions(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void performAuthenticatedRequest(std::function<void()> requestFunction);
  void console(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void getOrderBook(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
  void displayOrderIds(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

private:
  std::string accessToken;
  std::vector<std::string> orderIds;
};
