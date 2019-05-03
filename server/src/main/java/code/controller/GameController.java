package code.controller;

import code.entity.User;
import code.service.UserService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.ModelAndView;

import java.util.List;

@Controller
public class GameController {

    private final UserService service;

    public GameController(UserService service) {
        this.service = service;
    }

    @RequestMapping(value = "/")
    @ResponseBody
    public String postMyData() {

        return "Welcome";
    }
    @RequestMapping(value="/upload", method=RequestMethod.GET)
    public ModelAndView provideUploadInfo() {

        return new ModelAndView("upload");
    }

    @RequestMapping(value="/upload", method=RequestMethod.POST)
    public @ResponseBody String handleFileUpload(@RequestParam("file") MultipartFile file){
        return service.parseJson(file).toString();
    }
    @GetMapping("/getAll")
    @ResponseBody
    public List<User> getUsers() {
        return service.readAll();
    }
    @PostMapping("/getPage")
    @ResponseBody
    public List<User> getUsersPage(@RequestParam("limit") long limit) {
        return service.getPage(limit);
    }



}