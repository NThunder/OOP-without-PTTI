Геометрия

В этой задаче реализован набор классов для решения геометрических задач на плоскости.
Все координаты предполагаются целочисленными.
1) Класс Vector для вектора на плоскости.
Реализованы основные действия над векторами в виде методов и переопределения операций.
2)Созданы наборы классов фигур (в основе которых класс Vector)
для работы с двумерными геометрическими примитивами:
Point(точка);
Segment(отрезок);
Line(прямая);
Ray(луч);
Polygon(простой многоугольник - часть плоскости, ограниченная замкнутой ломаной без самопересечений);
Circle(окружность).
В каждом классе предусмотрены методы:
Move(const Vector&)– сдвиг на заданный вектор, метод изменяет состояние объекта и возвращать ссылку на сам объект;
ContainsPoint(const Point&)– проверки(true/false), содержит ли фигура точку;
CrossesSegment(const Segment&)– проверки(true/false), пересекается ли она с отрезком;
Clone()– копирования данного объекта (возвращается умный указатель на копию фигуры);
ToString()– строковое представление фигуры.

Все методы наборов классов фигур реализованны на основе похожих методов класса Vector

Также реализованы функции:

1) функция построения выпуклой оболочки void ConvexHull(std::vector<Vector>&)
(предпологается что оболочка не вырождена);

2) функция для вычисления площади выпуклого многоугольника с абсолютной точностью long double AreaPolygon(std::vector<Vector>&)
(предпологается что все координаты вершин многоугольника целочисленные);
