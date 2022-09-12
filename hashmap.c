#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr); //del codigo ASCI a Número
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}



void insertMap(HashMap * map, char * key, void * value) {
    Pair * p = createPair(key, value); //
    size_t i = hash(key, map->capacity); // punto donde debe ir guardado
  
  while (1){
    if (map->buckets[i] == NULL){
    map->buckets[i] = p; 
    map->size++;
    map->current = i;
    return;
    }
    i++;
    if (i == map->capacity){
      i = 0;
    }
  }  
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** aux = map->buckets; // variable auxiliar
    size_t capa = map->capacity;
    size_t i;

    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    map->size = 0;

    for (i = 0; i < capa; i++) {
        if (aux[i] != NULL) {
            insertMap(map, aux[i]->key, aux[i]->value);
        }
    }
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap*)malloc(sizeof(HashMap));
  
  map->buckets = (Pair **)malloc(capacity * sizeof(Pair));

  map->capacity = capacity;
  map->size = 0;
  map->current = -1;

  return map;
}

void eraseMap(HashMap * map,  char * key) {
  size_t i = hash(key, map->capacity);
  size_t cont = 0;

  if (map->buckets[i] == NULL) return;
  
  while(1){
    if (map->buckets[i] == NULL || map->buckets == NULL) return;    
    if (is_equal(key, map->buckets[i]->key)){
      map->size--;
      map->buckets[i]->value = NULL;
      map->buckets[i]->key = NULL;
    }
    i++;
    cont++;
    if (cont == map->capacity) break;
    if (i == map->capacity){
      i=0;
    }
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
  size_t i = hash(key, map->capacity);
  size_t cont = 0;

  while(1){
    if (map->buckets[i] == NULL || map->buckets == NULL) return NULL;    
    if (is_equal(key, map->buckets[i]->key)){
      map->current = i;
      return map->buckets[i];
    }
    i++;
    cont++;
    if (cont == map->capacity) break;
    if (i == map->capacity){
      i=0;
    }
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  size_t i = 0;

  while(1){
    if (map->buckets[i] == NULL || map->buckets == NULL) return NULL; // Si no hay lista, se va
    if (map->buckets[i] != NULL){ //si el primer dato no es NULL, entra
      map->current = i;
      return map->buckets[i];
    }
  }
  map->current = 0;
  return NULL;
}

Pair * nextMap(HashMap * map) {
  size_t i = 0;

  while(1){
    if (map->buckets[i+1] == NULL || map->buckets == NULL) return NULL; //si es nulo, se va
    if (map->buckets[i+1] != NULL){
      map->current = i;
      return map->buckets[i+1];
    }
    i++;
  }
  
    return NULL;
}
