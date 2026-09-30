#include <iostream>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
sf::RectangleShape cube({15.0f, 15.0f});

using Point2D = sf::Vector2f;

// TODO: (Part 1) Define a function that samples a cubic Bezier curve at t in [0, 1].
Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t)
{
    Point2D BezierCurve;
    BezierCurve.x = (std::pow(1-t, 3) * pts.at(0).x + t*pts.at(1).x*(3*std::pow(1-t, 2)) + pts.at(2).x*(3*(1-t)*std::pow(t, 2)) + pts.at(3).x*std::pow(t, 3));
    BezierCurve.y = (std::pow(1-t, 3) * pts.at(0).y + t*pts.at(1).y*(3*std::pow(1-t, 2)) + pts.at(2).y*(3*(1-t)*std::pow(t, 2)) + pts.at(3).y*std::pow(t, 3));
    return BezierCurve;
}

Point2D getPoint2Dots(const std::vector<sf::Vector2f>& pts, float t)
{
    Point2D BezierCurve;
    BezierCurve.x = (1-t) * pts.at(0).x + t * pts.at(1).x;
    BezierCurve.y = (1-t) * pts.at(0).y + t * pts.at(1).y;
    return BezierCurve;
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t)
{
    Point2D BezierSlope;
    BezierSlope.x = (3*std::pow(1-t, 2)*(pts.at(1).x - pts.at(0).x) + 6*(1 - t) * t * (pts.at(2).x - pts.at(1).x) + 3*std::pow(t, 2)*(pts.at(3).x - pts.at(2).x));
    BezierSlope.y = (3*std::pow(1-t, 2)*(pts.at(1).y - pts.at(0).y) + 6*(1 - t) * t * (pts.at(2).y - pts.at(1).y) + 3*std::pow(t, 2)*(pts.at(3).y - pts.at(2).y));
    return BezierSlope;
}

// TODO: (Part 1) Store four control points for the curve.
std::vector<sf::Vector2f> BezierCurvePoints;
Point2D POINT1(100.0f, 100.0f);
Point2D POINT2(100.0f, 700.0f);
Point2D POINT3(700.0f, 100.0f);
Point2D POINT4(700.0f, 700.0f);

// TODO: (Part 2) Track animation time for the square moving along the curve.
const float ANIMATION_FRAME_DURATION = 60;
float FrameCounter = 0;
int trackCounter = 1;
int CurrentTrack = 1;

// TODO: (Part 3) Track the index of the control point being dragged.
int currentControlPoint = -1;

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            // TODO: (Part 3) On left-click, select the closest control point
            // using mouse->position and start dragging it.
            Point2D closestPoint = BezierCurvePoints.at(0);
            float shortestDistance = sqrt(std::pow(mouse->position.x - closestPoint.x, 2) + std::pow(mouse->position.y - closestPoint.y, 2));
            int indexCounter = -1;
            currentControlPoint = 0;
            for (Point2D point : BezierCurvePoints)
            {
                indexCounter++;
                float pointDistance = sqrt(std::pow(mouse->position.x - point.x, 2) + std::pow(mouse->position.y - point.y, 2));
                if (point != closestPoint && pointDistance < shortestDistance)
                {
                    closestPoint = point;
                    shortestDistance = pointDistance;
                    currentControlPoint = indexCounter;
                }
            }
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            // TODO: (Part 3) On left-button release, stop dragging.
            currentControlPoint = -1;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
            // TODO: (Part 3) Move the selected control point to mouse->position.
            if (currentControlPoint != -1)
            {
                BezierCurvePoints.at(currentControlPoint).x = mouse->position.x;
                BezierCurvePoints.at(currentControlPoint).y = mouse->position.y;
            }
            // TODO: (Part 4) Maintain matching slopes at shared endpoints.
            // When moving point 3, move point 5 without changing its distance
            // from point 4 (point numbers here start at 1).

            if ((currentControlPoint % 3) != 0 && trackCounter > 1 && (currentControlPoint) != (BezierCurvePoints.size() - 1))
            {
                if ((currentControlPoint % 3) == 2 && currentControlPoint > 1 && (currentControlPoint + 1) != (BezierCurvePoints.size() - 1))
                {
                    //-----------------------------------------------------------------------
                    //Couldn't get code working properly for the smooth Bezier Curve Segments
                    //-----------------------------------------------------------------------

                    // float magnitude = sqrt(std::pow(BezierCurvePoints.at(currentControlPoint).x, 2) + std::pow(BezierCurvePoints.at(currentControlPoint).y, 2));
                    // Point2D normalizedPoint((BezierCurvePoints.at(currentControlPoint).x / magnitude), (BezierCurvePoints.at(currentControlPoint).y / magnitude));
                    // normalizedPoint -= BezierCurvePoints.at(currentControlPoint + 1);
                    // normalizedPoint.x = normalizedPoint.x * -1;
                    // normalizedPoint.y = normalizedPoint.y * -1;
                    // normalizedPoint += BezierCurvePoints.at(currentControlPoint + 1);
                    // float distance = sqrt(std::pow(BezierCurvePoints.at(currentControlPoint+1).x - BezierCurvePoints.at(currentControlPoint+2).x, 2) + std::pow(BezierCurvePoints.at(currentControlPoint+1).y - BezierCurvePoints.at(currentControlPoint+2).y, 2));
                    // BezierCurvePoints.at(currentControlPoint + 2).x = normalizedPoint.x * distance;
                    // BezierCurvePoints.at(currentControlPoint + 2).y = normalizedPoint.y * distance;
                }
                else if ((currentControlPoint % 3) == 1 && currentControlPoint > 1)
                {
                    //-----------------------------------------------------------------------
                    //Couldn't get code working properly for the smooth Bezier Curve Segments
                    //-----------------------------------------------------------------------

                    // float magnitude = sqrt(std::pow(BezierCurvePoints.at(currentControlPoint).x, 2) + std::pow(BezierCurvePoints.at(currentControlPoint).y, 2));
                    // Point2D normalizedPoint((BezierCurvePoints.at(currentControlPoint).x / magnitude), (BezierCurvePoints.at(currentControlPoint).y / magnitude));
                    // normalizedPoint -= BezierCurvePoints.at(currentControlPoint - 1);
                    // normalizedPoint.x = normalizedPoint.x * -1;
                    // normalizedPoint.y = normalizedPoint.y * -1;
                    // normalizedPoint += BezierCurvePoints.at(currentControlPoint - 1);
                    // float distance = sqrt(std::pow(BezierCurvePoints.at(currentControlPoint-1).x - BezierCurvePoints.at(currentControlPoint-2).x, 2) + std::pow(BezierCurvePoints.at(currentControlPoint-1).y - BezierCurvePoints.at(currentControlPoint-2).y, 2));
                    // BezierCurvePoints.at(currentControlPoint - 2).x = (normalizedPoint.x * distance);
                    // BezierCurvePoints.at(currentControlPoint - 2).y = (normalizedPoint.y * distance);
                }
            }
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            // TODO: (Part 4) '+' adds three control points; '-' removes three,
            // keeping at least four points.
            if (key->scancode == sf::Keyboard::Scancode::Equal)
            {
                Point2D oldPoint1 = BezierCurvePoints.back();
                Point2D oldPoint2 = getPoint(BezierCurvePoints, 0.90);
                std::vector<sf::Vector2f> tempCurvePoints;
                tempCurvePoints.push_back(oldPoint1);
                tempCurvePoints.push_back(oldPoint2);

                Point2D newPoint1 = getPoint2Dots(tempCurvePoints, 1.10);
                Point2D newPoint2 = getPoint2Dots(tempCurvePoints, 1.20);
                Point2D newPoint3 = getPoint2Dots(tempCurvePoints, 1.30);

                BezierCurvePoints.push_back(newPoint1);
                BezierCurvePoints.push_back(newPoint2);
                BezierCurvePoints.push_back(newPoint3);

                trackCounter++;
            }
            if (key->scancode == sf::Keyboard::Scancode::Hyphen)
            {
                if (trackCounter > 1)
                {
                    BezierCurvePoints.pop_back();
                    BezierCurvePoints.pop_back();
                    BezierCurvePoints.pop_back();
                    CurrentTrack = 1;
                    FrameCounter = 0;
                    trackCounter--;
                }
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Part 1) Sample GetPoint over t in [0, 1] and connect samples using the line-drawing
    // code from your project. Draw all four control points as circles after drawing the curve.
    // ====== ====== ======

    float resolution = 200.0f;
    sf::VertexArray functionLine(sf::PrimitiveType::LineStrip, resolution);
    for (int i = 0; i < resolution; i++)
    {
        Point2D bezierPoint = getPoint(BezierCurvePoints, i/resolution);

        functionLine[i].position = bezierPoint;
        functionLine[i].color = sf::Color::Cyan;
    }
    window.draw(functionLine);

    if (BezierCurvePoints.size() > 4)
    {
        int ExtraCurveNum = (BezierCurvePoints.size() - 4) / 3;
        for (int i = 0; i < ExtraCurveNum; i++)
        {
            float resolution = 200.0f;
            sf::VertexArray functionLine(sf::PrimitiveType::LineStrip, resolution);
            std::vector<sf::Vector2f> tempCurvePoints;
            tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 3));
            tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 4));
            tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 5));
            tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 6));
            for (int j = 0; j < resolution; j++)
            {
                Point2D bezierPoint = getPoint(tempCurvePoints, j/resolution);
                functionLine[j].position = bezierPoint;
                functionLine[j].color = sf::Color::Cyan;
            }
            tempCurvePoints.clear();
            window.draw(functionLine);
        }
    }

    for (sf::Vector2f point : BezierCurvePoints)
    {
        sf::CircleShape dot(7.5f);
        dot.setOrigin({7.5f, 7.5f});
        dot.setPosition(point);
        dot.setFillColor(sf::Color::Blue);
        window.draw(dot);
    }
    

    // ====== ====== ======
    // TODO: (Part 2) Draw a small square moving repeatedly along the curve.
    // Use GetSlope to orient it to the curve at each time step.
    // ====== ====== ======
    
    if (trackCounter == 1)
    {
        Point2D bezierPoint = getPoint(BezierCurvePoints, FrameCounter/ANIMATION_FRAME_DURATION);
        cube.setPosition(bezierPoint);
        Point2D bezierSlope = getSlope(BezierCurvePoints, FrameCounter/ANIMATION_FRAME_DURATION);
        sf::Angle angle = sf::radians(atan2(bezierSlope.y, bezierSlope.x));
        cube.setRotation(angle);
        window.draw(cube);
    }
    else
    {   
        int i = CurrentTrack - 1;
        std::vector<sf::Vector2f> tempCurvePoints;
        tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 0));
        tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 1));
        tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 2));
        tempCurvePoints.push_back(BezierCurvePoints.at(i * 3 + 3));
        Point2D bezierPoint = getPoint(tempCurvePoints, FrameCounter/ANIMATION_FRAME_DURATION);
        cube.setPosition(bezierPoint);
        Point2D bezierSlope = getSlope(tempCurvePoints, FrameCounter/ANIMATION_FRAME_DURATION);
        sf::Angle angle = sf::radians(atan2(bezierSlope.y, bezierSlope.x));
        cube.setRotation(angle);
        window.draw(cube);
        tempCurvePoints.clear();
    }
    

    // ====== ====== ======
    // TODO: (Part 3) Draw control handles from point 1 to 2 and point 3 to 4.
    // TODO: (Part 4) Draw all connected cubic Bezier segments and their handles.
    // ====== ====== ======
    int lineNum = BezierCurvePoints.size()/2;
    for (int i = 0; i < lineNum; i++)
    {
        float resolution = 50.0f;
        sf::VertexArray functionLine(sf::PrimitiveType::LineStrip, resolution);
        std::vector<sf::Vector2f> tempLinePoints;
        tempLinePoints.push_back(BezierCurvePoints.at(i*2));
        tempLinePoints.push_back(BezierCurvePoints.at(i*2 + 1));
        for (int j = 0; j < resolution; j++)
        {
            Point2D bezierPoint = getPoint2Dots(tempLinePoints, j/resolution);
            functionLine[j].position = bezierPoint;
            functionLine[j].color = sf::Color::Red;
        }
        tempLinePoints.clear();
        window.draw(functionLine);
    }

    // ====== ====== ======
    // TODO: (Bonus) Support multiple curves, a Galaga screen overlay at a 1:2 ratio, and exporting
    // curve points as C++ code for Project 1b.
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;
    BezierCurvePoints.push_back(POINT1);
    BezierCurvePoints.push_back(POINT2);
    BezierCurvePoints.push_back(POINT3);
    BezierCurvePoints.push_back(POINT4);

    cube.setFillColor(sf::Color::Yellow);
    cube.setOrigin({7.5f, 7.5f});

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Bezier Curve Editor");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            if (FrameCounter > ANIMATION_FRAME_DURATION && CurrentTrack != trackCounter)
            {
                FrameCounter = 0;
                CurrentTrack++;
            }
            else if (FrameCounter > ANIMATION_FRAME_DURATION && CurrentTrack == trackCounter)
            {
                FrameCounter = 0;
                CurrentTrack = 1;
            }
            render(window);
            FrameCounter++;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
