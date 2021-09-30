import { Component, OnInit } from '@angular/core';
import {ControleService} from "./controle.service";

@Component({
  selector: 'app-controle',
  templateUrl: './controle.component.html',
  styleUrls: ['./controle.component.css']
})
export class ControleComponent implements OnInit {

  constructor(private controleService: ControleService) { }

  ngOnInit(): void {
  }

  sendMn() {
    this.controleService.sendMoveCommand(":Mn#").subscribe((retorno: string)=> console.log(retorno));
  }
  sendMs() {
    this.controleService.sendMoveCommand(":Ms#").subscribe((retorno: string)=> console.log(retorno));
  }
  sendMw() {
    this.controleService.sendMoveCommand(":Mw#").subscribe((retorno: string)=> console.log(retorno));
  }
  sendMe() {
    this.controleService.sendMoveCommand(":Me#").subscribe((retorno: string)=> console.log(retorno));
  }
  sendQ() {
    this.controleService.sendMoveCommand(":Q#").subscribe((retorno: string)=> console.log(retorno));
  }
}
