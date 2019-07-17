typedef struct Element Element;
struct Element
{
    int val;
    Element *suivant;
};
typedef struct Pile Pile;
struct Pile
{
    Element *premier;
};
////empiler
void empiler(Pile *pile, int nouvNombre)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (pile == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->val = nouvNombre;
    nouveau->suivant = pile->premier;
    pile->premier = nouveau;
}
//depiler
int depiler(Pile *pile)
{
    if (pile == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int valDepile = 0;
    Element *elementDepile = pile->premier;

    if (pile != NULL && pile->premier != NULL)
    {
        valDepile = elementDepile->val;
        pile->premier = elementDepile->suivant;
        free(elementDepile);
    }

    return valDepile;
}
