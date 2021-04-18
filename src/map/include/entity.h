#pragma once

#include <entt.hpp>
#include <spdlog/fmt/fmt.h>

using Entity = entt::entity;
using Registry = entt::registry;

template<>
struct fmt::formatter<Entity>
{
  template<typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
      return ctx.begin();
  }

  template<typename FormatContext>
  auto format(Entity const& entity, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "{}", entt::to_integral(entity));
  }
};
