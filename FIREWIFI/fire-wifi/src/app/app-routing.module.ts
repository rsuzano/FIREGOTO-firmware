import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {ControleComponent} from "./controle/controle.component";

const routes: Routes = [{path:'controle', component: ControleComponent},
                        {path:'', component: ControleComponent}
                        ];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
