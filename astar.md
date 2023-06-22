// A* Search Algorithm
1.  Initialize the open list

2.  Initialize the closed list
    - put the starting Node on the open list (you can leave its F at zero)

3.  while the OPEN list is not empty
    1. find the Node with the least F on the OPEN list, call it "Current"

    2. pop Current off the OPEN list

    3. generate Current's successors, set their parents to Current

    4. for each Successor
       1. if Successor is the goal, stop search

       2. else, compute both G, H and F for Successor:
            - Successor.G = Current.G + distance between Successor and Current
            - Successor.H = distance from goal to Successor 
            - Successor.F = Successor.G + Successor.H

        3. if a Node with the same position as Successor is in the CLOSED list which has a lower F than Successor, skip this Successor

        4. if a Node with the same position as Successor is in the OPEN list and has a lower F than Successor, skip this Successor

        5. if a Node with the same position as Successor is in the OPEN list and has a higher F than Successor, pop Node from OPEN list and push Successor

        6. otherwise, push the Node to the OPEN list

    END (for loop)

    5. push Current on the CLOSED list
END (while loop)