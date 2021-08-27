#include "ReimpQGraphicsScene.h"
#include "mainwindow.h"

ReimpQGraphicsScene::ReimpQGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::white);
}

void ReimpQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(EmptyNodeExists() && playing)
    {
        QPointF pointClick;
        pointClick = mouseEvent->buttonDownScenePos(Qt::LeftButton);
        areaState = outOfRange;
        //X dots on certain columns
        bool firstColumnDotsOnX = pointClick.x() > -360 && pointClick.x() < -240;
        bool secondColumnDotsOnX = pointClick.x() > -240 && pointClick.x() < -120;
        bool thirdColumnDotsOnX = pointClick.x() > -120 && pointClick.x() < 0;
        //Y dots on certain rows
        bool firstRowDotsOnY = pointClick.y() > -180 && pointClick.y() < -60;
        bool secondRowDotsOnY = pointClick.y() > -60 && pointClick.y() < 60;
        bool thirdRowDotsOnY = pointClick.y() > 60 && pointClick.y() < 180;
        //Area over the grid
        bool areaOverGridOnX = pointClick.x() < -360 xor pointClick.x() > 0;
        bool areaOverGridOnY = pointClick.y() < -180 xor pointClick.y() > 180;
        //Choosing state for different area node
        this->StateSelectOnFirstRow(firstRowDotsOnY, firstColumnDotsOnX,
                              secondColumnDotsOnX, thirdColumnDotsOnX);
        this->StateSelectOnSecondRow(secondRowDotsOnY, firstColumnDotsOnX,
                               secondColumnDotsOnX, thirdColumnDotsOnX);
        this->StateSelectOnThirdRow(thirdRowDotsOnY, firstColumnDotsOnX,
                              secondColumnDotsOnX, thirdColumnDotsOnX);
        if(areaOverGridOnY && areaOverGridOnX) areaState = outOfRange;
        if(areaState != outOfRange) this->SetCrossArea();
        this->SeekForWinningState();
    }
}

void ReimpQGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(EmptyNodeExists() && playing)
    {
        QVector<int> nextMoveOnNode;
        QVector<int> occupiedNodeNum = OccupiedNodeNumbers();
        for(int i = 0; i < currentGridState.size(); i++)
        {
            for(int j = 0; j < currentGridState[i].size(); j++)
            {
                if(currentGridState[i][j] == empty)
                    nextMoveOnNode.push_back(ConvertIndexesToNodeNum(i,j));
            }
        }
        std::random_shuffle(nextMoveOnNode.begin(),nextMoveOnNode.end());
        areaState = ConvertIntToAreaState(nextMoveOnNode[0]);
        this->SetCircleArea();
        this->SeekForWinningState();
    }
}

void ReimpQGraphicsScene::DrawGrid()
{
    QPen pen(Qt::black);
    pen.setWidth(6);
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            this->addRect(-360 + 120 * j,-180 + 120 * i, 120, 120, pen);
        }
    }
}

void ReimpQGraphicsScene::SetCrossArea()
{
    auto drawCross = [=](int xStart1End2Line, int yStart, int xStart2End1Line,
                                                int yEnd, int i, int j)
            {
                if(currentGridState[i][j] == empty)
                {
                    this->addLine(xStart1End2Line, yStart, xStart2End1Line, yEnd, pen);
                    this->addLine(xStart2End1Line, yStart, xStart1End2Line, yEnd, pen);
                    currentGridState[i][j] = cross;
                }
            };
    switch (areaState)
    {
    case firstRectangle:
        drawCross(-330, -150, -270, -90, 0, 0);
        break;
    case secondRectangle:
        drawCross(-210, -150, -150, -90, 0, 1);
        break;
    case thirdRectangle:
        drawCross(-90, -150, -30, -90, 0, 2);
        break;
    case fourthRectangle:
        drawCross(-330, -30, -270, 30, 1, 0);
        break;
    case fifthRectangle:
        drawCross(-210, -30, -150, 30, 1, 1);
        break;
    case sixthRectangle:
        drawCross(-30, -30, -90, 30, 1, 2);
        break;
    case seventhRectangle:
        drawCross(-330, 90, -270, 150, 2, 0);
        break;
    case eighthRectangle:
        drawCross(-210, 90, -150, 150, 2, 1);
        break;
    case ninthRectangle:
        drawCross(-90, 90, -30, 150, 2, 2);
        break;
    case outOfRange:
        break;
    }
}

void ReimpQGraphicsScene::SetCircleArea()
{
    auto drawCircle = [=](int x, int y,int i, int j)
    {
        if(currentGridState[i][j] == empty)
        {
            this->addEllipse(x, y, 87, 87, pen);
            currentGridState[i][j] = circle;
        }
    };
    switch (areaState)
    {
    case firstRectangle:
        drawCircle(-343, -165, 0, 0);
        break;
    case secondRectangle:
        drawCircle(-223, -165, 0, 1);
        break;
    case thirdRectangle:
        drawCircle(-103, -165, 0, 2);
        break;
    case fourthRectangle:
        drawCircle(-343, -45, 1, 0);
        break;
    case fifthRectangle:
        drawCircle(-223, -45, 1, 1);
        break;
    case sixthRectangle:
        drawCircle(-103, -45, 1, 2);
        break;
    case seventhRectangle:
        drawCircle(-343, 75, 2, 0);
        break;
    case eighthRectangle:
        drawCircle(-223, 75, 2, 1);
        break;
    case ninthRectangle:
        drawCircle(-103, -45, 2, 2);
        break;
    case outOfRange:
        break;
    }
}

void ReimpQGraphicsScene::SetPen()
{
    pen.setColor("black");
    pen.setWidth(6);
}

inline void ReimpQGraphicsScene::StateSelectOnFirstRow(bool firstRowDotsOnY, bool firstColumnDotsOnX,
                                                bool secondColumnDotsOnX, bool thirdColumnDotsOnX)
{
    if(firstRowDotsOnY && firstColumnDotsOnX) areaState = firstRectangle;
    if(firstRowDotsOnY && secondColumnDotsOnX) areaState = secondRectangle;
    if(firstRowDotsOnY && thirdColumnDotsOnX) areaState = thirdRectangle;
}

inline void ReimpQGraphicsScene::StateSelectOnSecondRow(bool secondRowDotsOnY, bool firstColumnDotsOnX,
                                                 bool secondColumnDotsOnX, bool thirdColumnDotsOnX)
{
    if(secondRowDotsOnY && firstColumnDotsOnX) areaState = fourthRectangle;
    if(secondRowDotsOnY && secondColumnDotsOnX) areaState = fifthRectangle;
    if(secondRowDotsOnY && thirdColumnDotsOnX) areaState = sixthRectangle;
}

inline void ReimpQGraphicsScene::StateSelectOnThirdRow(bool thirdRowDotsOnY, bool firstColumnDotsOnX,
                                                bool secondColumnDotsOnX, bool thirdColumnDotsOnX)
{
    if(thirdRowDotsOnY && firstColumnDotsOnX) areaState = seventhRectangle;
    if(thirdRowDotsOnY && secondColumnDotsOnX) areaState = eighthRectangle;
    if(thirdRowDotsOnY && thirdColumnDotsOnX) areaState = ninthRectangle;
}

QVector<int> ReimpQGraphicsScene::OccupiedNodeNumbers()
{
    QVector<int> occupiedNodes;
    for(int i = 0; i < currentGridState.size(); i++)
    {
        for(int j = 0; j < currentGridState[i].size(); j++)
        {
            int crossIndex;
            if(currentGridState[i][j] == cross)
            {
                crossIndex = ConvertIndexesToNodeNum(i, j);
                occupiedNodes.push_back(crossIndex);
            }
        }
    }
    return occupiedNodes;
}

int ReimpQGraphicsScene::ConvertIndexesToNodeNum(int i, int j)
{
    QVector<QVector<AreaState>> areaNum = {{firstRectangle, secondRectangle, thirdRectangle},
                                             {fourthRectangle, fifthRectangle, sixthRectangle},
                                             {seventhRectangle, eighthRectangle, ninthRectangle}};
    int nodeNum;
    for(int n = 0; n < areaNum.size(); n++)
    {
        for(int m = 0; m < areaNum[n].size(); m++)
        {
            if(i == n && j == m) nodeNum = areaNum[i][j];
        }
    }
    return nodeNum;
}

AreaState ReimpQGraphicsScene::ConvertIntToAreaState(int rectNum)
{
    QVector<AreaState> areaNum = {firstRectangle, secondRectangle, thirdRectangle,
                                             fourthRectangle, fifthRectangle, sixthRectangle,
                                             seventhRectangle, eighthRectangle, ninthRectangle};
    AreaState outputState;
    for(auto i : areaNum)
    {
        if(i == rectNum) outputState = i;
    }
    return outputState;
}

bool ReimpQGraphicsScene::EmptyNodeExists()
{
    int emptyNodeCounter = 0;
    for(int i = 0; i < currentGridState.size(); i++)
    {
        for(int j = 0; j < currentGridState[i].size(); j++)
        {
            if(currentGridState[i][j] == empty) emptyNodeCounter++;
        }
    }
    return emptyNodeCounter > 0 ? true : false;
}

void ReimpQGraphicsScene::SeekForWinningState()
{
    if(HorizontalWinning() xor VerticalWinning() xor DiagonalWinning())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("End of game");
        QString whoWin = currentWinner == player ? "Player" : "Computer";
        msgBox.setText(whoWin + " is winner");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

bool ReimpQGraphicsScene::HorizontalWinning()
{
    QVector<NodeState> playerWin = {cross, cross, cross};
    QVector<NodeState> computerWin = {circle, circle, circle};
    for(const auto vecState : currentGridState)
    {
        if(vecState == playerWin) return SetWinnerAndPlayingState(player);
        if(vecState == computerWin) return SetWinnerAndPlayingState(computer);
    }
    return false;
}

bool ReimpQGraphicsScene::VerticalWinning()
{
    for (int i = 0; i < currentGridState.size(); i++)
    {
        int crossCounter = 0, circleCounter = 0;
        CountCrossCirclesOnLine(0, i, crossCounter, circleCounter);
        CountCrossCirclesOnLine(1, i, crossCounter, circleCounter);
        CountCrossCirclesOnLine(2, i, crossCounter, circleCounter);
        if(crossCounter == 3) return SetWinnerAndPlayingState(player);
        if(circleCounter == 3) return SetWinnerAndPlayingState(computer);
    }
    return false;
}

bool ReimpQGraphicsScene::DiagonalWinning()
{
    auto WinStateExists = [=](bool isMainDiagonal) -> bool {
        int crossCounter = 0, circleCounter = 0;
        int j;
        for(int i = 0; i < currentGridState.size(); i++)
        {
            j = isMainDiagonal == true ? i : currentGridState.size() - i - 1;
            if(currentGridState[i][j] == cross) crossCounter++;
            if(currentGridState[i][j] == circle) circleCounter++;
        }
        if(crossCounter == 3) return SetWinnerAndPlayingState(player);
        if(circleCounter == 3) return SetWinnerAndPlayingState(computer);
        return false;
    };
    if(WinStateExists(true)) return true;
    if(WinStateExists(false)) return true;
    return false;
}

bool ReimpQGraphicsScene::SetWinnerAndPlayingState(Winner winner)
{
    currentWinner = winner;
    playing = false;
    return true;
}

void ReimpQGraphicsScene::CountCrossCirclesOnLine(int i, int j, int &crossCounter, int &circleCounter)
{
    if(currentGridState[i][j] == cross) crossCounter++;
    if(currentGridState[i][j] == circle) circleCounter++;
}

