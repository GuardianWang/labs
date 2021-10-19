#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QColor>

/**

    @struct Settings
    @brief  Stores application settings for the CS123 GUI.

    You can access all app settings through the "settings" global variable.
    The settings will be automatically updated when things are changed in the
    GUI (the reverse is not true however: changing the value of a setting does
    not update the GUI).

*/
struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    // Saves the current settings to disk.
    void saveSettings();

    float lightIntensity,
          attQuadratic,
          attLinear,
          attConstant,
          ambientIntensity,
          diffuseIntensity,
          specularIntensity,
          shininess;

    QColor lightColor;
    QColor sphereLColor;
    QColor sphereMColor;
    QColor sphereRColor;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
