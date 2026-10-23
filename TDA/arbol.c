#include "arbol.h"


void crearArbol(tArbol *pa)
{
    *pa=NULL;
}
void recorrerPreOrdenArbol(tArbol *pa, void(mostrar)(const void *elem))
{//Rid
    if(*pa)
    {
        mostrar((*pa)->info);
        recorrerPreOrdenArbol(&(*pa)->izq, mostrar);
        recorrerPreOrdenArbol(&(*pa)->der, mostrar);
    }
}
void recorrerInOrdenArbol(tArbol *pa, void(mostrar)(const void *elem))
{//iRd
    if(*pa)
    {
        recorrerInOrdenArbol(&(*pa)->izq, mostrar);
        mostrar((*pa)->info);
        recorrerInOrdenArbol(&(*pa)->der, mostrar);
    }
}
void recorrerPosOrdenArbol(tArbol *pa, void(mostrar)(const void *elem))
{//idR
    if(*pa)
    {
        recorrerPosOrdenArbol(&(*pa)->izq, mostrar);
        recorrerPosOrdenArbol(&(*pa)->der, mostrar);
        mostrar((*pa)->info);
    }
}
int insertarOrdenadoConRecursividad(tArbol *pa, const void *elem, unsigned tam, int(comparacion)(const void *elem1, const void *elem2))
{
    tNodo *nue;
    int comp;
    if(*pa==NULL)
    {
        nue=malloc(sizeof(tNodo));
        if(!nue)
            return ERROR_MEMORIA;
        nue->info=malloc(tam);
        if(!nue->info)
        {
            free(nue);
            return ERROR_MEMORIA;
        }

        memcpy(nue->info, elem, tam);
        nue->tamElem=tam;
        nue->izq=nue->der=NULL;
        *pa=nue;

        return TODO_OK;
    }
    else
    {
        comp=comparacion((*pa)->info, elem);
        if(comp==0)
            return DUPLICADO;
        else
            if(comp<0)//el elem a insertar es mayor que la raiz, voy a le derecha
                return insertarOrdenadoConRecursividad(&(*pa)->der, elem, tam, comparacion);
            else
                return insertarOrdenadoConRecursividad(&(*pa)->izq, elem, tam, comparacion);
    }
}
int insertarOrdenadoSinRecursividad(tArbol *pa, const void *elem, unsigned tam, int(comparacion)(const void *elem1, const void *elem2))
{
    tNodo *nue;
    int comp;

    while(*pa)
    {
        comp=comparacion((*pa)->info, elem);
        if(comp==0)
            return DUPLICADO;
        else
            if(comp<0)
                pa=&(*pa)->der;
            else
                pa=&(*pa)->izq;
    }

    nue=malloc(sizeof(tNodo));
    if(!nue)
        return ERROR_MEMORIA;
    nue->info=malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return ERROR_MEMORIA;
    }

    memcpy(nue->info, elem, tam);
    nue->tamElem=tam;
    nue->izq=nue->der=NULL;
    *pa=nue;

    return TODO_OK;

}

int contarNodosDeArbol(tArbol *pa)
{//Recorremos con preOrden: Rid
    return _contarNodosDeArbol(pa, 0);
}
int _contarNodosDeArbol(tArbol *pa, int nodos)
{
    if(*pa)
    {
        nodos++;//Hay un nodo raiz, lo cuento
        nodos = _contarNodosDeArbol(&(*pa)->izq, nodos);
        nodos = _contarNodosDeArbol(&(*pa)->der, nodos);

    }
    return nodos;
}

int contarNodosConHijosporIzqDeArbol(tArbol *pa)
{
    return _contarNodosConHijosporIzqDeArbol(pa, 0);
}
int _contarNodosConHijosporIzqDeArbol(tArbol *pa, int nodos)
{
    if(*pa)
    {
        if((*pa)->izq)
            nodos++;//Hay un nodo raiz con hijo por izquierda, lo cuento
        nodos = _contarNodosConHijosporIzqDeArbol(&(*pa)->izq, nodos);
        nodos = _contarNodosConHijosporIzqDeArbol(&(*pa)->der, nodos);

    }
    return nodos;
}

int contarNodosConHijosSOLOporIzqDeArbol(tArbol *pa)
{
    return _contarNodosConHijosSOLOporIzqDeArbol(pa, 0);
}
int _contarNodosConHijosSOLOporIzqDeArbol(tArbol *pa, int nodos)
{
    if(*pa)
    {
        if((*pa)->izq && !(*pa)->der)
            nodos++;
        nodos = _contarNodosConHijosSOLOporIzqDeArbol(&(*pa)->izq, nodos);
        nodos = _contarNodosConHijosSOLOporIzqDeArbol(&(*pa)->der, nodos);
    }
    return nodos;
}

int contarNodosConHijosporIzqyDerDeArbol(tArbol *pa)
{
    return _contarNodosConHijosporIzqyDerDeArbol(pa, 0);
}
int _contarNodosConHijosporIzqyDerDeArbol(tArbol *pa, int nodos)
{
    if(*pa)
    {
        if((*pa)->izq && (*pa)->der)
            nodos++;
        nodos = _contarNodosConHijosporIzqyDerDeArbol(&(*pa)->izq, nodos);
        nodos = _contarNodosConHijosporIzqyDerDeArbol(&(*pa)->der, nodos);
    }
    return nodos;
}

int contarHojasDeArbol(tArbol *pa)
{
    if(!pa || !(*pa))
        return 0;

    if(!(*pa)->izq && !(*pa)->der)
        return 1;

    return contarHojasDeArbol(&(*pa)->izq)+contarHojasDeArbol(&(*pa)->der);
}
int calcularAlturadelArbol(tArbol *pa)
{
    return _calcularAlturadelArbol(pa, 0);
}
int _calcularAlturadelArbol(tArbol *pa, int altura)
{//Hay que sacar la maxima altura entre izq o der
    int altIzq=altura, altDer=altura;
    if(*pa)
    {
        altura++;
        altIzq=_calcularAlturadelArbol(&(*pa)->izq, altura);
        altDer=_calcularAlturadelArbol(&(*pa)->der, altura);
        altura=MAYOR(altIzq, altDer);

    }
    return altura;
}

int contarNodosEnUnaAltura(tArbol *pa, int altura)
{
    return _contarNodosEnUnaAltura(pa, altura, 0, 0);
}
int _contarNodosEnUnaAltura(tArbol *pa, int altura, int altActual, int nodos)
{
    if(*pa)
    {
        altActual++;
        if(altActual==altura)
            nodos++;
        nodos=_contarNodosEnUnaAltura(&(*pa)->izq, altura, altActual, nodos);
        nodos=_contarNodosEnUnaAltura(&(*pa)->der, altura, altActual, nodos);
    }
    return nodos;
}

int contarNodosHastaUnaAltura(tArbol *pa, int altura)
{
    return _contarNodosHastaUnaAltura(pa, altura, 0, 0);
}
int _contarNodosHastaUnaAltura(tArbol *pa, int altura, int altActual, int nodos)
{
    if(*pa && altActual<=altura)
    {
        altActual++;
        if(altActual<=altura)
            nodos++;
        nodos=_contarNodosHastaUnaAltura(&(*pa)->izq, altura, altActual, nodos);
        nodos=_contarNodosHastaUnaAltura(&(*pa)->der, altura, altActual, nodos);
    }
    return nodos;
}

int contarNodosAPartirdeUnaAltura(tArbol *pa, int altura)
{
    return  _contarNodosAPartirdeUnaAltura(pa, altura, 0, 0);
}
int _contarNodosAPartirdeUnaAltura(tArbol *pa, int altura, int altActual, int nodos)
{
    if(*pa)
    {
        if(altActual>=altura)
            nodos++;//Hay un nodo raiz, lo cuento
        nodos = _contarNodosAPartirdeUnaAltura(&(*pa)->izq, altura, altActual, nodos);
        nodos = _contarNodosAPartirdeUnaAltura(&(*pa)->der, altura, altActual, nodos);

    }
    return nodos;
}

void cortarHojas(tArbol *pa)
{
    if(*pa)
    {
        if(!(*pa)->izq && !(*pa)->der)
        {
            free((*pa)->info);
            free(*pa);
            *pa=NULL;
        }
        else
        {
            cortarHojas(&(*pa)->izq);
            cortarHojas(&(*pa)->der);
        }
    }
}
int cortarNodo(tArbol *pa, const void *elem, int(cmp)(const void *elem1, const void *elem2))
{
    return 1;
}

int generarArbolAPartirDeArchivoBinario(tArbol *pa, const char *nomArch, unsigned tamElem,int(cmp)(const void *elem1, const void *elem2))
{
    FILE *pf=fopen(nomArch, "rb");
    tArbol *ini=pa;
    void *aux;
    tNodo *nue;
    int comparacion;
    if(!pf)
        return ERROR_ARCHIVO;

    aux=malloc(tamElem);
    if(!aux)
    {
        fclose(pf);
        return ERROR_MEMORIA;
    }
    fread(aux, tamElem, 1, pf);
    while(!feof(pf))
    {
        pa=ini;
        comparacion=1;
        while(*pa)
        {
            comparacion=cmp((*pa)->info, aux);
            if(comparacion>0)
                pa=&(*pa)->izq;
            else
                pa=&(*pa)->der;
        }
        if(comparacion)
        {
            nue=malloc(sizeof(tNodo));
            if(!nue)
            {
                free(aux);
                fclose(pf);
                return ERROR_MEMORIA;
            }
            nue->tamElem=tamElem;
            nue->info=aux;
            nue->der=NULL;
            nue->izq=NULL;
            *pa=nue;
            //Reservo memoria aca, ya que si no entrase aca quiere decir que el elemento esta duplicado y puedo reutilizar esa dir de memoria
            aux=malloc(tamElem);
            if(!aux)
            {
                fclose(pf);
                return ERROR_MEMORIA;
            }
        }
        fread(aux, tamElem, 1, pf);
    }

    fclose(pf);
    return TODO_OK;//No libero void *aux ya que sino estaria liberando el ultimo nue->info que guarde
}
int generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(tArbol *pa, const char *nomArch, unsigned tamElem)
{
    FILE *pf=fopen(nomArch, "rb");
    int cantElems, ret;

    if(!pf)
        return ERROR_ARCHIVO;

    fseek(pf, 0L, SEEK_END);
    cantElems=ftell(pf)/tamElem;
    rewind(pf);

    ret=_generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(pa, tamElem, pf, 0, cantElems-1);
    if(ret!=TODO_OK)
        return ret;
    fclose(pf);
    return TODO_OK;
}
int _generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(tArbol *pa, unsigned tamElem, FILE *pf, int ini, int fin)
{
    tNodo *nue;
    int li=ini, ls=fin, medio;

    if(li>ls)
        return TODO_OK;
    nue=malloc(sizeof(tNodo));
    if(!nue)
        return ERROR_MEMORIA;
    nue->info=malloc(tamElem);
    if(!nue->info)
    {
        free(nue);
        return ERROR_MEMORIA;
    }
    *pa=nue;
    nue->tamElem=tamElem;
    nue->izq=nue->der=NULL;

    medio=(li+ls)/2;

    fseek(pf, medio*tamElem, SEEK_SET);
    fread(nue->info, tamElem, 1, pf);

    if(_generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(&(*pa)->izq, tamElem, pf, ini, medio-1)!=TODO_OK)
        return ERROR_MEMORIA;
    if(_generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(&(*pa)->der, tamElem, pf, medio+1, fin)!=TODO_OK)
        return ERROR_MEMORIA;

    return TODO_OK;
}
int crearArchivoBinarioAPartirDeArbol(tArbol *pa, const char *nomarch)
{
    FILE *pf=fopen(nomarch, "wb");
    if(!pf)
        return ERROR_ARCHIVO;
    _crearArchivoBinarioAPartirDeArbol(pa, pf);

    fclose(pf);
    return TODO_OK;
}
void _crearArchivoBinarioAPartirDeArbol(tArbol *pa, FILE *pf)
{//Si o si para cargar en archivo recorrido inOrden: iRd
    if(*pa)
    {
        _crearArchivoBinarioAPartirDeArbol(&(*pa)->izq, pf);
        fwrite((*pa)->info, (*pa)->tamElem, 1, pf);
        _crearArchivoBinarioAPartirDeArbol(&(*pa)->der, pf);
    }
}
int eliminarElemDeArbol(tArbol *pa, void *elem, unsigned tamElem,int(cmp)(const void *elem1, const void *elem2))//no repetir codigo
{
    tArbol *buscado;
    if(!(*pa) || !pa)//Si apunta a nulo, retorno que el arbol esta vacio
        return ARBOL_VACIO;
    buscado=buscarElemEnArbol(pa, elem, cmp);
    if(buscado)
    {
        if((*buscado)->der && (*buscado)->izq)//Posee hijo por izquierda y derecha
            return eliminarRaizDeArbol(buscado, elem, tamElem, cmp);
        else
            return eliminarNodo(pa, elem, buscado);
    }
    else
        return NO_ENCONTRADO;
}
tArbol* buscarElemEnArbol(tArbol *pa, void *elem, int(cmp)(const void *elem1, const void *elem2))//se puede hacer de forma iterativa
{
    int comparacion;
    if(!(*pa))//No lo encontre, retorno nulo
        return NULL;

    comparacion=cmp((*pa)->info, elem);
    if(comparacion==0)
        return pa;
    else
        if(comparacion<0)//Busco por derecha del arbol
            return buscarElemEnArbol(&(*pa)->der, elem, cmp);
        else
            return buscarElemEnArbol(&(*pa)->izq, elem, cmp);//Sino debo buscar por izquierda del arbol
}
int eliminarRaizDeArbol(tArbol *pa, void *elem, unsigned tamElem,int(cmp)(const void *elem1, const void *elem2))//Debo buscar mayor y menor, todo eso
{
    tNodo *elim;
    tArbol *aux;
    if(calcularAlturadelArbol(&(*pa)->izq) > calcularAlturadelArbol(&(*pa)->der))//si la altura del arbol izquierdo es mayor, debo buscar el mayor de los menores
    {
        aux=BuscarMayorClave(&(*pa)->izq);//Busco el mayor de los menores, retorno la direccion de lo que lo apunta
        elim=*aux;
        memcpy(elem, (*pa)->info, MINIMO((*pa)->tamElem, tamElem));
        //free((*pa)->info); //Libero la info de la raiz
        memcpy((*pa)->info, elim->info, tamElem);
        *aux=elim->izq;
    }
    else
    {//Debo buscar el menor de los mayores
        aux=BuscarMenorClave(&(*pa)->der);
        elim=*aux;
        memcpy(elem, (*pa)->info, MINIMO((*pa)->tamElem, tamElem));
        //free((*pa)->info); //Libero la info de la raiz
        memcpy((*pa)->info, elim->info, tamElem);
        *aux=elim->der;
    }
    free(elim->info);
    free(elim);

    return TODO_OK;

}
int eliminarNodo(tArbol *pa, void *elem, tArbol *aEliminar)
{
    //En esta funcion: tiene hijo por izq o der, o es hoja

    tNodo *elim=*aEliminar;

    if(elim->der && !elim->izq)//Si tiene hijo por derecha pero no por izquierda
    {
        *aEliminar=elim->der;
    }
    else
        if(!elim->der && elim->izq)
        {
            *aEliminar=elim->izq;
        }
        else//es hoja
            *aEliminar=NULL;

    free(elim->info);
    free(elim);

    return TODO_OK;
}
tArbol* BuscarMayorClave(tArbol *pa)
{
    while((*pa)->der)
    {
        pa=&(*pa)->der;
    }
    return pa;
}
tArbol* BuscarMenorClave(tArbol *pa)
{
    while((*pa)->izq)
    {
        pa=&(*pa)->izq;
    }
    return pa;
}
void destruirArbol(tArbol *pa)
{//Si o si en idR (posorden)
    if(*pa)
    {
        destruirArbol(&(*pa)->izq);
        destruirArbol(&(*pa)->der);
        free((*pa)->info);
        free(*pa);
        *pa=NULL;
    }
}
