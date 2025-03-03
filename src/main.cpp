#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <boost/describe.hpp>
#include <boost/mp11/algorithm.hpp>
#include <string>
#include <cstring>
#include <type_traits>

// Helper to display different types in ImGui
template <typename T>
void ShowImGuiWidget(const char* label, T& value);

template <>
void ShowImGuiWidget<int>(const char* label, int& value)
{
    ImGui::InputInt(label, &value);
}

template <>
void ShowImGuiWidget<float>(const char* label, float& value)
{
    ImGui::InputFloat(label, &value);
}

template <>
void ShowImGuiWidget<std::string>(const char* label, std::string& value)
{
    char buffer[256];
    std::strcpy(buffer, value.c_str());
    if (ImGui::InputText(label, buffer, sizeof(buffer)))
    {
        value = buffer;
    }
}

// Sample reflected class
struct MyActor
{
    int health = 100;
    float damage = 5.0f;
    std::string name = "Actor";

    // Non-reflected member (won't be exposed)
    float internalTimer = 0.0f;

    float selamlar = 31;
};

// Register the members using Boost.Describe
BOOST_DESCRIBE_STRUCT(MyActor, (), (health, damage, name, selamlar))

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ImGui Reflection Example");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    MyActor actor;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        // Create an ImGui window to display the reflected members.
        ImGui::Begin("Actor Properties");
        boost::mp11::mp_for_each<boost::describe::describe_members<MyActor, boost::describe::mod_public>>([&](auto member_descriptor)
        {
            // member_descriptor.pointer is a pointer-to-member
            constexpr const char* name = decltype(member_descriptor)::name;
            auto& member_value = actor.*member_descriptor.pointer;
            ShowImGuiWidget(name, member_value);
        });
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
