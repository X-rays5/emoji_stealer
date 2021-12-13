#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "api.hpp"

std::string GetGuildName(const std::string& guild_id) {
    rapidjson::Document json;
    if (!json.Parse(bot::api::ApiGet(fmt::format("guilds/{}", guild_id)).text.c_str()).HasParseError()) {
      return json["name"].GetString();
    } else {
      std::cerr << "Error: " << json.GetParseError() << std::endl;
      return {};
    }
}

struct Emoji {
  std::string id;
  bool animated;
};

std::vector<Emoji> GetEmojis(const std::string& guild_id) {
  rapidjson::Document json;
  if (!json.Parse(bot::api::ApiGet(fmt::format("guilds/{}/emojis", guild_id)).text.c_str()).HasParseError()) {

    std::vector<Emoji> emojis;
    for (rapidjson::SizeType i = 0; i< json.Size(); i++) {
      emojis.push_back({json[i]["id"].GetString(), json[i]["animated"].GetBool()});
    }
    return emojis;
  } else {
    std::cerr << "Error: " << json.GetParseError() << std::endl;
  }
}

int main() {
  std::cout << "Enter token: ";
  std::string token;
  std::getline(std::cin, token);

  bot::api::Token::SetToken(token);

  rapidjson::Document json;
  if (!json.Parse(bot::api::ApiGet("users/@me").text.c_str()).HasParseError()) {
    std::cout << "Logged in as: " << json["username"].GetString() << '#' << json["discriminator"].GetString() << std::endl;

    std::cout << "Enter guild id: ";
    std::string guild_id;
    std::getline(std::cin, guild_id);

    std::cout << "Guild name: " << GetGuildName(guild_id) << std::endl;

    if (!std::filesystem::is_directory(guild_id))
      std::filesystem::create_directory(guild_id);

    std::vector<std::string> extensions = {"png", "jpg", "jpeg"};
    std::uint32_t download_count = 0;
    auto emojis = GetEmojis(guild_id);
    for (auto&& emoji : emojis) {
      if (emoji.animated) {
        std::ofstream writer(fmt::format("{0}/{1}.gif", guild_id, emoji.id), std::ios::binary);
        cpr::Response r = cpr::Download(writer, cpr::Url{fmt::format("https://cdn.discordapp.com/emojis/{0}.gif", emoji.id)});
        writer.close();

        if (r.status_code == 200) {
          std::cout << "Downloaded emoji: " << emoji.id << std::endl;
          download_count += 1;
        } else {
          std::cerr << "Failed to download: " << emoji.id << '\n' << "Status code: " << r.status_code << '\n' << r.text << std::endl;
        }
      } else {
        for (auto&& extension : extensions) {
          std::ofstream writer(fmt::format("{0}/{1}.{2}", guild_id, emoji.id, extension), std::ios::binary);
          cpr::Response r = cpr::Download(writer, cpr::Url{fmt::format("https://cdn.discordapp.com/emojis/{0}.{1}", emoji.id, extension)});
          writer.close();

          if (r.status_code == 200) {
            std::cout << "Downloaded emoji: " << emoji.id << std::endl;
            download_count += 1;
            break;
          } else {
            std::cerr << "Failed to download: " << emoji.id << '\n' << "Status code: " << r.status_code << '\n' << r.text << std::endl;
          }
        }
      }
    }
    std::cout << fmt::format("Downloaded {0} out of {1}", download_count, emojis.size()) << "\n";
  } else {
    std::cerr << "Error: " << json.GetParseError() << std::endl;
  }
  return 0;
}
