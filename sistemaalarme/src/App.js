import {BrowserRouter as Router, Routes, Route} from 'react-router-dom'
import NewTime from './components/pages/NewTime';
import Time from './components/pages/Time';
import Login from './components/pages/Login';
import Container from './components/layouts/Container';
import NavBar from './components/layouts/NavBar';

import Alarm from './components/pages/Alarm';

function App() {

  return (
    <>
       <Router>
       <NavBar/>
         <Container customClass='min-heigth'>
           <Routes>
              <Route exact path='/ProjetoSirene' element={<Login/>}/>
              <Route exact path='/newTime'  element={<NewTime/>}/>
              <Route exact path='/times'  element={<Time/>}/>
              <Route exact path='/time/:id' element={<Alarm/>}/>
           </Routes>
       </Container>
       </Router>
    
  </>
  )
}
export default App;

