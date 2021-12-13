//
// Created by X-ray on 8/20/2021.
//

#pragma once

#ifndef DISCORD_CPP_BOT_API_HPP
#define DISCORD_CPP_BOT_API_HPP
#include <utility>
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace bot {
  namespace api {
    class Token {
    public:
      Token(const Token&) = delete;

      static Token& Get() {
        static Token instance;
        return instance;
      }

      inline static std::string GetToken() {
        return Get().IGetToken();
      }

      inline static void SetToken(const std::string& token) {
        Get().ISetToken(token);
      }

    private:
      Token() = default;

      std::string IGetToken() {
        return token_;
      }

      void ISetToken(const std::string& token) {
        token_ = token;
      }
    private:
      std::string token_;
    };

    inline std::string JsonStringify(rapidjson::Document& json) {
      rapidjson::StringBuffer strbuf;
      strbuf.Clear();

      rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
      json.Accept(writer);

      return strbuf.GetString();
    }

    inline cpr::Response ApiGet(const std::string& url) {
      return cpr::Get(cpr::Url{fmt::format("https://discordapp.com/api/v9/{0}", url)}, cpr::Header{{"authorization", Token::GetToken()}, {"Content-Type", "Application/Json"}});
    }

    inline cpr::Response ApiPost(const std::string& url, const std::string& body) {
      return cpr::Post(cpr::Url{fmt::format("https://discordapp.com/api/v9/{0}", url)}, cpr::Body{body}, cpr::Header{{"authorization", Token::GetToken()}, {"Content-Type", "Application/Json"}});
    }

    inline cpr::Response ApiPatch(const std::string& url, const std::string& body) {
      return cpr::Patch(cpr::Url{fmt::format("https://discordapp.com/api/v9/{0}", url)}, cpr::Body{body}, cpr::Header{{"authorization", Token::GetToken()}, {"Content-Type", "Application/Json"}});
    }

    inline cpr::Response ApiDelete(const std::string& url) {
      return cpr::Delete(cpr::Url{fmt::format("https://discordapp.com/api/v9/{0}", url)}, cpr::Header{{"authorization", Token::GetToken()}, {"Content-Type", "Application/Json"}});
    }
  }
}
#endif //DISCORD_CPP_BOT_API_HPP