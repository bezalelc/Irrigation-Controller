import { BrowserRouter, Route, Routes } from 'react-router-dom';
import { FirebaseProvider } from './FirebaseContext';
import Navbar from './components/Navbar';
import Login from './pages/Login';
import Register from './pages/Register';
import Home from './pages/Home';
import Setting from './pages/Setting';
import PrivateRoute from './pages/PrivateRoute'
import ForgotPassword from './pages/ForgotPassword';
import './App.css';

function App() {
  return (
    <FirebaseProvider>
      <BrowserRouter>
        <Navbar />
        <Routes>
          <Route path='/' element={<Login />} />
          <Route exact path='/register' element={<Register />} />
          <Route exact path='/forgotPassword' element={<ForgotPassword />} />
          <Route exact path="/home" element={<PrivateRoute component={Home} />} />
          <Route exact path="/setting" element={<PrivateRoute component={Setting} />} />
        </Routes>
      </BrowserRouter>
    </FirebaseProvider>
  );
}

export default App;
