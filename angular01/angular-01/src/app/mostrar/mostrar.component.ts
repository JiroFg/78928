import { Component } from '@angular/core';
import { DataService } from '../data.service';

@Component({
  selector: 'app-mostrar',
  templateUrl: './mostrar.component.html',
  styleUrls: ['./mostrar.component.css']
})
export class MostrarComponent {
  items: any[] = [];

  constructor (private ds:DataService){}

  ngOnInit(){
    this.ds.mostrar().subscribe(datos =>{
      this.items = datos;
      console.log('datos'+datos);
    });
  }
}
