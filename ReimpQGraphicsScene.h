#ifndef REIMPQGRAPHICSVIEW_H
#define REIMPQGRAPHICSVIEW_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <algorithm>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <numeric>

enum AreaState
{
   outOfRange,
   firstRectangle,
   secondRectangle,
   thirdRectangle,
   fourthRectangle,
   fifthRectangle,
   sixthRectangle,
   seventhRectangle,
   eighthRectangle,
   ninthRectangle
};

enum NodeState
{
    empty,
    cross,
    circle
};

enum Winner
{
    player,
    computer,
    draw
};

class ReimpQGraphicsScene : public QGraphicsScene
{
 Q_OBJECT
 public:
    explicit ReimpQGraphicsScene(QObject *parent = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void DrawGrid();
    void SetCrossArea();
    void SetCircleArea();
    void SetPen();
 private:
    AreaState areaState;
    QPen pen;
    QVector<QVector<NodeState>> currentGridState = {{empty, empty, empty},
                                                    {empty, empty, empty},
                                                    {empty, empty, empty}};
    Winner currentWinner;
    bool playing = true;

    inline void StateSelectOnFirstRow(bool firstRowDotsOnY, bool firstColumnDotsOnX,
                               bool secondColumnDotsOnX, bool thirdColumnDotsOnX);
    inline void StateSelectOnSecondRow(bool secondRowDotsOnY, bool firstColumnDotsOnX,
                                bool secondColumnDotsOnX, bool thirdColumnDotsOnX);
    inline void StateSelectOnThirdRow(bool thirdRowDotsOnY, bool firstColumnDotsOnX,
                               bool secondColumnDotsOnX, bool thirdColumnDotsOnX);
    QVector<int> OccupiedNodeNumbers();
    int ConvertIndexesToNodeNum(int i, int j);
    AreaState ConvertIntToAreaState(int rectNum);
    bool EmptyNodeExists();
    void SeekForWinningState();
    bool HorizontalWinning();
    bool VerticalWinning();
    bool DiagonalWinning();
    bool SetWinnerAndPlayingState(Winner winner);
    void CountCrossCirclesOnLine(int i, int j, int &crossCounter, int &circleCounter);
};

#endif // REIMPQGRAPHICSVIEW_H
