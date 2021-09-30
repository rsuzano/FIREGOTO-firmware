import {NgModule} from '@angular/core';
import {BrowserModule} from '@angular/platform-browser';

import {AppRoutingModule} from './app-routing.module';
import {AppComponent} from './app.component';
import {NgbModule} from '@ng-bootstrap/ng-bootstrap';
import {FaIconLibrary, FontAwesomeModule} from '@fortawesome/angular-fontawesome';
import {faArrowDown, faArrowLeft, faArrowUp, faArrowRight, faBan} from "@fortawesome/free-solid-svg-icons";
import {MenuComponent} from "./menu/menu.component";
import { ControleComponent } from './controle/controle.component';
import {HttpClientModule} from "@angular/common/http";

@NgModule({
  declarations: [
    AppComponent,
    MenuComponent,
    ControleComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    AppRoutingModule,
    NgbModule,
    FontAwesomeModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule {
  constructor(private faLibrary: FaIconLibrary) {
    faLibrary.addIcons(faArrowUp);
    faLibrary.addIcons(faArrowDown);
    faLibrary.addIcons(faArrowLeft);
    faLibrary.addIcons(faArrowRight);
    faLibrary.addIcons(faBan);
  }
}
