#include "Line.h"
#include <imgui.h>
#include "UIManager.h"
#include "Structure/GameObject.h"
#include "Structure/Scene.h"

namespace Tapioca {
Line::Line()
    : Component(), startPosition(Vector2()), endPosition(Vector2(100.0f, 100.0f)),
      lineColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)), lineThickness(5.0f), addBorder(false),
      borderColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f)), borderThickness(1.0f), uiManager(nullptr) { }

Line::~Line() { uiManager = nullptr; }

void Line::start() { uiManager = UIManager::instance(); }

bool Line::initComponent(const CompMap& variables) {
    if (!setValueFromMap(startPosition.x, "startPositionX", variables)) {
        logInfo(("Line: No se encontro el valor de startPositionX. Se inicializo al valor predefinido: \"" +
                 std::to_string(startPosition.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(startPosition.y, "startPositionY", variables)) {
        logInfo(("Line: No se encontro el valor de startPositionY. Se inicializo al valor predefinido: \"" +
                 std::to_string(startPosition.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(endPosition.x, "endPositionX", variables)) {
        logInfo(("Line: No se encontro el valor de endPositionX. Se inicializo al valor predefinido: \"" +
                 std::to_string(endPosition.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(endPosition.y, "endPositionY", variables)) {
        logInfo(("Line: No se encontro el valor de endPositionY. Se inicializo al valor predefinido: \"" +
                 std::to_string(endPosition.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(lineColor.x, "lineColorR", variables)) {
        logInfo(("Line: No se encontro el valor de lineColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(lineColor.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(lineColor.y, "lineColorG", variables)) {
        logInfo(("Line: No se encontro el valor de lineColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(lineColor.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(lineColor.z, "lineColorB", variables)) {
        logInfo(("Line: No se encontro el valor de lineColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(lineColor.z) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(lineColor.w, "lineColorA", variables)) {
        logInfo(("Line: No se encontro el valor de lineColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(lineColor.w) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(lineThickness, "lineThickness", variables)) {
        logInfo(("Line: No se encontro el valor de lineThickness. Se inicializo al valor predefinido: \"" +
                 std::to_string(lineThickness) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(addBorder, "addBorder", variables)) {
        logInfo(("Line: No se encontro el valor de addBorder. Se inicializo al valor predefinido: \"" +
                 std::to_string(addBorder) + "\".")
                    .c_str());
    }

    if (addBorder) {
        if (!setValueFromMap(borderColor.x, "borderColorR", variables)) {
            logInfo(("Line: No se encontro el valor de borderColorR. Se inicializo al valor predefinido: \"" +
                     std::to_string(borderColor.x) + "\".")
                        .c_str());
        }

        if (!setValueFromMap(borderColor.y, "borderColorG", variables)) {
            logInfo(("Line: No se encontro el valor de borderColorG. Se inicializo al valor predefinido: \"" +
                     std::to_string(borderColor.y) + "\".")
                        .c_str());
        }

        if (!setValueFromMap(borderColor.z, "borderColorB", variables)) {
            logInfo(("Line: No se encontro el valor de borderColorB. Se inicializo al valor predefinido: \"" +
                     std::to_string(borderColor.z) + "\".")
                        .c_str());
        }

        if (!setValueFromMap(borderColor.w, "borderColorA", variables)) {
            logInfo(("Line: No se encontro el valor de borderColorA. Se inicializo al valor predefinido: \"" +
                     std::to_string(borderColor.w) + "\".")
                        .c_str());
        }

        if (!setValueFromMap(borderThickness, "borderThickness", variables)) {
            logInfo(("Line: No se encontro el valor de borderThickness. Se inicializo al valor predefinido: \"" +
                     std::to_string(borderThickness) + "\".")
                        .c_str());
        }
        else if (borderThickness <= 0) {
            logWarn(
                "Line: El grosor del borde no puede ser menor o igual a 0. Se inicializo al valor predefinido: 1.0.");
            borderThickness = 1.0f;
        }
    }
    return true;
}

void Line::render() const {
    float scaleFactorX = object->getScene()->getScaleFactorX();
    float scaleFactorY = object->getScene()->getScaleFactorY();

    ImVec2 lineStartPos(startPosition.x * scaleFactorX, startPosition.y * scaleFactorY);
    ImVec2 lineEndPos(endPosition.x * scaleFactorX, endPosition.y * scaleFactorY);
    float lineThicknessScaled = lineThickness * (scaleFactorX + scaleFactorY) / 2.0;
    float borderThicknessScaled = borderThickness * (scaleFactorX + scaleFactorY) / 2.0;

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    ImU32 line = IM_COL32(lineColor.x * 255, lineColor.y * 255, lineColor.z * 255, lineColor.w * 255);
    if (addBorder) {
        ImU32 border = IM_COL32(borderColor.x * 255, borderColor.y * 255, borderColor.z * 255, borderColor.w * 255);
        drawList->AddLine(lineStartPos, lineEndPos, border, borderThicknessScaled + lineThicknessScaled);
    }
    drawList->AddLine(lineStartPos, lineEndPos, line, lineThicknessScaled);
}
}
