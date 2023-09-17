import { BrowserRouter, Route, Routes } from 'react-router-dom';
import { FirebaseProvider } from './FirebaseContext';
import Navbar from './navbar/Navbar';
import Login from './loginRegister/Login';
import Register from './loginRegister/Register';
import ForgotPassword from './loginRegister/ForgotPassword';
import PrivateRoute from './pages/PrivateRoute'
import Home from './home/Home';
import Setting from './setting/Setting';
import Config from './config/Config';
import './App.scss';
import AddArea from './AddArea/AddArea';

function App() {
  return (
    <FirebaseProvider>
      <BrowserRouter>
        {/* <img src="/resources/gif/drop.gif" alt="" id="background-gif"></img> */}
        <video autoPlay loop muted playsInline preload="auto" id="background-gif">
          <source src="/resources/video/background_2k.mp4" type="video/mp4" />
        </video>
        <Navbar />
        <Routes>
          <Route path='/' element={<Login />} />
          <Route exact path='/register' element={<Register />} />
          <Route exact path='/forgotPassword' element={<ForgotPassword />} />
          <Route exact path="/addarea" element={<PrivateRoute component={AddArea} />} />
          <Route exact path="/home" element={<PrivateRoute component={Home} />} />
          <Route exact path="/setting" element={<PrivateRoute component={Setting} />} />
          <Route exact path="/config" element={<PrivateRoute component={Config} />} />
        </Routes>
      </BrowserRouter>
    </FirebaseProvider>
  );
}

export default App;
