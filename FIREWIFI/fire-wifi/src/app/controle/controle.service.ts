import {Injectable} from '@angular/core';
import {HttpClient} from "@angular/common/http";
import {environment} from "../../environments/environment";

@Injectable({
  providedIn: 'root'
})
export class ControleService {

  constructor(private http: HttpClient) {
  }

  sendMoveCommand(command: string) {
    return this.http.get<string>(environment.ip + "/command/?cmd=" + command.replace("#","%23"));
  }
}
