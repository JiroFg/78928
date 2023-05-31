import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { map } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DataService {

  items: any[] = [];

  constructor(private http: HttpClient) {}

  salvar(saludador:String[]){
    this.http.post<any>('https://pruebaangular-50b82-default-rtdb.firebaseio.com/saludador.json',saludador)
    .subscribe(
      response => console.log ('respuesta '+response.name),
      error => console.log ('error '+error.error)
    )
  }

  mostrar(): Observable<any[]>{
    return this.http.get('https://pruebaangular-50b82-default-rtdb.firebaseio.com/saludador.json').pipe(
      map((responseData: any) => {
        const items = [];
        for (const key in responseData){
          if(responseData.hasOwnProperty(key)){
            items.push({id:key, ...responseData[key]});
          }
        }
        return items;
      })
    )
    //.subscribe(items=>{console.log(items);});
  }
}
