#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>

// SpriteSheet ������ ������ �� �������� � ������ ���������������-������
// � ���������� ����������� (��������).
struct SpriteSheet {
    // ������� �� ���������? � ���. �����, ����� Texture2D ��� ������ SpriteSheet.
    const Texture2D* tex = nullptr;

    // ����� (��������������) � ����������� ��������.
    std::vector<Rectangle> frames;

    // �������������� "����� ������" -> ������ � frames (������ ��� UI/��������).
    std::unordered_map<std::string, int> named;

    void clear() { frames.clear(); named.clear(); tex = nullptr; }

    // �������� ���� �������, ������� ��� ������.
    int addFrame(Rectangle r) {
        frames.push_back(r);
        return static_cast<int>(frames.size() - 1);
    }

    // ��������� ��� ������������� �����.
    void nameFrame(const std::string& name, int index) {
        if (index >= 0 && index < (int)frames.size()) named[name] = index;
    }

    // �������� ������ ����� �� �����; -1 ���� �� ������.
    int indexOf(const std::string& name) const {
        auto it = named.find(name);
        return (it == named.end()) ? -1 : it->second;
    }
};
