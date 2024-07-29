import React, { useState } from 'react';
import styles from './Login.module.css'
import {VscKey, VscAccount} from 'react-icons/vsc'
import { useNavigate } from 'react-router-dom';
import Container from '../layouts/Container';
function Login(){
 
    const navigate = useNavigate()
    const [username, setUsername] = useState();
    const [password, setPassword] = useState();

    function handleSubmit(e){
      e.preventDefault();
      if (username === 'Admin' && password === 'Admin') {
        console.log('Usuário e senha corretos')
        navigate("/times")
        
      } else {
        alert('Credenciais inválidas');
      }
    }
    return (
      <>
      <Container customClass='min-heigth'>
      <div className={styles.container}>
          <form onSubmit={handleSubmit} className={styles.form}>
            <h2 className={styles.tituloLogin}>Login</h2>
                  <div className={styles.campo}>
                    <VscAccount className={styles.icon}/>
                    <input type="text" className={styles.input} placeholder="Digite seu nome de usuário" onChange={(e)=> setUsername(e.target.value)}/>
                  </div>
                  <div className={styles.campo}>
                      <VscKey className={styles.icon}/>
                      <input type="password" className={styles.input} placeholder="Digite sua senha" onChange={(e)=> setPassword(e.target.value)}/>
                  </div>
                  <input type="submit" value="Entrar" className={styles.btn_Entrar} />
                  
          </form>

          
        </div>
      </Container>
        
      </>
    );
  };
  
  export default Login;